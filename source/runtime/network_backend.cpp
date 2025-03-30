#include "network_backend.h"
#include <format>

#ifdef RYN_PLATFORM_WIN
#define WIN32_LEAN_AND_MEAN  // Exclude rarely used Windows stuff
#define _WINSOCKAPI_         // Prevent winsock.h from being included
#include <winsock2.h>
#include <WS2tcpip.h>
#endif

using namespace ryn::net::backend;

void ryn::net::backend::initialize() {
#ifdef RYN_PLATFORM_WIN
    // Startup WSA (Windows)
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        throw std::exception("Failed to initialize WinSock\n");
#endif
}

void ryn::net::backend::cleanup() {
#ifdef RYN_PLATFORM_WIN
    // Shutdown WSA (Windows)
    WSACleanup();
#endif
}

const uint16_t FAMILY = AF_INET; // Indicate IPv4 addresses
NetAddress::NetAddress(const char* ipv4, uint16_t port) {
    setIP(ipv4);
    setPort(port);
}

NetAddress::NetAddress() {
    port = 0;
    addr = 0;
}

void NetAddress::setIP(const char* ip) {
    if (inet_pton(FAMILY, ip, &addr) <= 0)
        throw std::invalid_argument("Invalid IP address format.");
}

void NetAddress::setPort(uint16_t port) { 
    this->port = htons(port);
}

string NetAddress::getIP(bool includePort) const {
    // Convert network byte order ip to string
    char ip[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &addr, ip, sizeof(ip)) == nullptr)
        return std::string("Couldn't convert binary IP to string.");

    // Return ip (with port if requested)
    if (includePort)
        return std::format("{}:{}", ip, getPort());
    return std::string(ip);
}

uint16_t NetAddress::getPort() const {
    return ntohs(port);
}

void NetAddress::setInternal(const sockaddr_in& address) {
    port = address.sin_port;
    addr = address.sin_addr.s_addr;
}

sockaddr_in NetAddress::getInternal() const {
    sockaddr_in in;
    in.sin_family = FAMILY;   
    in.sin_port = port;       
    in.sin_addr.s_addr = addr;
    return in;
}

UDPSocket::UDPSocket(uint16_t portBinding, bool blockingMode) : socketHandle(INVALID_SOCKET_HANDLE) {
    // Create UDP socket
    socketHandle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socketHandle == INVALID_SOCKET_HANDLE)
        throw std::exception("Failed to create a UDP socket.");

    // Configure socket with non-blocking mode
#ifdef RYN_PLATFORM_WIN
    if (!blockingMode) {
        u_long mode = 1;
        if (ioctlsocket(socketHandle, FIONBIO, &mode) != 0)
            throw std::exception("Failed to set socket to non-blocking mode.");
    }
#endif

    // Configure port binding
    sockaddr_in addr = {};
    addr.sin_family = FAMILY;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(portBinding);

#ifdef RYN_PLATFORM_WIN
    const int BIND_ERROR_CODE = SOCKET_ERROR;
#endif

    // Ensure creation of socket was successful
    if (bind(socketHandle, (sockaddr*)&addr, sizeof(addr)) == BIND_ERROR_CODE)
        throw std::exception("Failed to bind UDP socket.");
}

UDPSocket::~UDPSocket() {
    if (socketHandle != INVALID_SOCKET_HANDLE) {
#ifdef RYN_PLATFORM_WIN
        closesocket(socketHandle);
#endif
    }
}

int UDPSocket::send(const void* buffer, int32_t bufferSize, const NetAddress& toAddress) {
    // Ensure valid socket and parameters
    if (socketHandle == INVALID_SOCKET_HANDLE || buffer == nullptr || bufferSize == 0)
        return -1;

    // Send data
    sockaddr_in toAddr = toAddress.getInternal();
    int bytesSent = sendto(socketHandle, static_cast<const char*>(buffer), bufferSize, 0, reinterpret_cast<sockaddr*>(&toAddr), sizeof(toAddr));

    // Check for errors
#ifdef RYN_PLATFORM_WIN
    if (bytesSent == SOCKET_ERROR)
        switch (WSAGetLastError()) {
        case WSAEWOULDBLOCK: return 0;  // OS buffer full, retry later (sent 0 bytes)
        default: return -1;             // Other error, indicate failure
        }
#endif

    return bytesSent;
}

int UDPSocket::receive(void* buffer, int32_t bufferSize, NetAddress* fromAddress) {
    // Ensure valid socket and parameters
    if (socketHandle == INVALID_SOCKET_HANDLE || buffer == nullptr || bufferSize == 0)
        return -1;

    // Receive data and sender information
    sockaddr_in fromAddr;
    int fromAddrSize = sizeof(fromAddr);
    int bytesReceived = recvfrom(socketHandle, static_cast<char*>(buffer), bufferSize, 0, reinterpret_cast<sockaddr*>(&fromAddr), &fromAddrSize);

    // Check for errors
#ifdef RYN_PLATFORM_WIN
    if (bytesReceived == SOCKET_ERROR)
        switch (WSAGetLastError()) {
        case WSAEWOULDBLOCK: return 0;  // OS buffer empty, retry later (received 0 bytes)
        default: return -1;             // Other error, indicate failure
        }
#endif

    // If caller requested sender address, populate 'fromAddress' object
    if (fromAddress != nullptr)
        fromAddress->setInternal(fromAddr);

    return bytesReceived;
}
