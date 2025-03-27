#pragma once

#include "public/defines.h"

namespace ryn::net::backend {
#ifdef RYN_PLATFORM_WIN
	using SocketHandle = uintptr_t; // Match Windows SOCKET type, an unsigned integer that adapts to the system's native word size.
	static constexpr SocketHandle INVALID_SOCKET_HANDLE = (SocketHandle)(~0); // Match Windows INVALID_SOCKET definition, represents an invalid socket handle.
#else
#error "Network sockets are currently only supported on Windows. Please define RYN_PLATFORM_WIN or implement support for other platforms."
#endif // RYN_PLATFORM_WIN

	void initialize();
	void cleanup();

	struct NetAddress {
		const uint32_t netIPv4;
		const uint16_t netPort;

		NetAddress(const char* ipv4, uint16_t port);
		~NetAddress() = default;

		string getIPHostRepresentation(bool includePort = false) const;
		uint16_t getPortHostRepresentation() const;
	};

	class UDPSocket {
	public:
		UDPSocket(uint16_t portBinding = 0, bool blockingMode = true);
		~UDPSocket();

		int send(const void* buffer, int32_t bufferSize, const NetAddress& toAddress);
		int receive(void* buffer, int32_t bufferSize);

	private:
		SocketHandle socketHandle;
	};
}
