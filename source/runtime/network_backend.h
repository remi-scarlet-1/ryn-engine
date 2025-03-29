#pragma once
// TODO: document file and classes. Warn how this backend is not threadsafe, especially UDPSocket!

#include "public/defines.h"

// Forward declarations
struct sockaddr_in;

namespace ryn::net::backend {
#ifdef RYN_PLATFORM_WIN
	using SocketHandle = uintptr_t;												// Match Windows SOCKET type, an unsigned integer that adapts to the system's native word size.
	static constexpr SocketHandle INVALID_SOCKET_HANDLE = (SocketHandle)(~0);	// Match Windows INVALID_SOCKET definition, represents an invalid socket handle.
#endif

	void initialize();
	void cleanup();

	struct NetAddress {
		NetAddress(const char* ipv4, uint16_t port);
		NetAddress();
		~NetAddress() = default;

		void setIP(const char* ipv4);
		void setPort(uint16_t port);
		void setInternal(const sockaddr_in& address);

		string getIP(bool includePort = false) const;
		uint16_t getPort() const;
		sockaddr_in getInternal() const;

	private:
		uint16_t family;
		uint16_t port;
		uint32_t addr;
	};

	class UDPSocket {
	public:
		UDPSocket(uint16_t portBinding = 0, bool blockingMode = false);
		~UDPSocket();

		int send(const void* buffer, int32_t bufferSize, const NetAddress& toAddress);
		int receive(void* buffer, int32_t bufferSize, NetAddress* fromAddress = nullptr);

	private:
		SocketHandle socketHandle;
	};
}
