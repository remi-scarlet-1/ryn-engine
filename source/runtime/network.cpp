#include "network.h"

#ifdef RYN_PLATFORM_WIN
#define WIN32_LEAN_AND_MEAN  // Exclude rarely used Windows stuff
#define _WINSOCKAPI_         // Prevent winsock.h from being included
#include <winsock2.h>
#include <WS2tcpip.h>
#endif // RYN_PLATFORM_WIN

using namespace ryn::net;

void ryn::net::initialize() {
#ifdef RYN_PLATFORM_WIN
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        throw std::exception("Failed to initialize WinSock\n");
#endif // RYN_PLATFORM_WIN
}

void ryn::net::cleanup() {
#ifdef RYN_PLATFORM_WIN
    WSACleanup();
#endif // RYN_PLATFORM_WIN
}

NetAddress::NetAddress(const char* ipv4, uint16_t port)
    : netIPv4([ipv4]{ 
            uint32_t addr; 
            inet_pton(AF_INET, ipv4, &addr); 
            return addr; 
        }()), 
      netPort(htons(port)) 
{ }

string NetAddress::getIPHostRepresentation(bool includePort) const {
    struct in_addr addr;
    addr.s_addr = netIPv4;

    // Convert address bytes to ip string
    char ip[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &addr, ip, sizeof(ip)) == nullptr)
        return string("Couldn't convert net IP to string.");
    
    string result = ip;
    
    // Convert port bytes and append to string if requested
    if (includePort)
        result.append(":").append(std::to_string(ntohs(netPort)));

    return result;
}

uint16_t NetAddress::getPortHostRepresentation() const {
    return ntohs(netPort);
}

UDPSocket::UDPSocket(uint16_t portBinding) : socketHandle(INVALID_SOCKET_HANDLE) {
    // Create UDP socket
    socketHandle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socketHandle == INVALID_SOCKET_HANDLE)
        throw std::exception("Failed to create a UDP socket.");

    // Configure socket with non-blocking mode
#ifdef RYN_PLATFORM_WIN
    u_long mode = 1;
    if (ioctlsocket(socketHandle, FIONBIO, &mode) != 0)
        throw std::exception("Failed to set socket to non-blocking mode.");
#endif // RYN_PLATFORM_WIN

    // Configure port binding
    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY; 
    addr.sin_port = htons(portBinding);

    if (bind(socketHandle, (sockaddr*)&addr, sizeof(addr))
#ifdef RYN_PLATFORM_WIN
        == SOCKET_ERROR
#endif
    )
        throw std::exception("Failed to bind UDP socket.");
}

UDPSocket::~UDPSocket() {
    // Close UDP socket
    if (socketHandle != INVALID_SOCKET_HANDLE) {
#ifdef RYN_PLATFORM_WIN
        closesocket(socketHandle);
#endif // RYN_PLATFORM_WIN
    }
}

int UDPSocket::send(const void* buffer, int32_t bufferSize, const NetAddress& toAddress) {
    if (socketHandle == INVALID_SOCKET_HANDLE || buffer == nullptr || bufferSize == 0)
        return -1; // Invalid parameters or socket
    
    // Send data
    sockaddr_in toAddr = {};
    toAddr.sin_family = AF_INET;
    toAddr.sin_addr.s_addr = toAddress.netIPv4;
    toAddr.sin_port = toAddress.netPort;
    int bytesSent = sendto(socketHandle, static_cast<const char*>(buffer), bufferSize, 0, reinterpret_cast<sockaddr*>(&toAddr), sizeof(toAddr));

    // Check for errors
#ifdef RYN_PLATFORM_WIN
    if (bytesSent == SOCKET_ERROR)
        switch (WSAGetLastError()) {
            case WSAEWOULDBLOCK: return 0; // OS buffer is full, caller should retry later (sent 0 bytes).  
            default: return -1; // Other error, indicate failure
        }
#endif // RYN_PLATFORM_WIN

    // Return number of bytes actually sent
    return bytesSent;
}

// TODO: optionally return somehow sender information
int UDPSocket::receive(void* buffer, int32_t bufferSize) {
    if (socketHandle == INVALID_SOCKET_HANDLE || buffer == nullptr || bufferSize == 0)
        return -1; // Invalid parameters or socket

    // Receive data & sender information
    sockaddr_in fromAddr;
    int fromAddrSize = sizeof(fromAddr);
    int bytesReceived = recvfrom(socketHandle, static_cast<char*>(buffer), bufferSize, 0, reinterpret_cast<sockaddr*>(&fromAddr), &fromAddrSize);

    // Check for errors
#ifdef RYN_PLATFORM_WIN
    if (bytesReceived == SOCKET_ERROR)
        switch (WSAGetLastError()) {
            case WSAEWOULDBLOCK: return 0; // No data available right now, caller should try again later (received 0 bytes)
            default: return -1; // Other error, indicate failure
        }
#endif // RYN_PLATFORM_WIN

    // Return number of bytes actually received
    return bytesReceived;
}
