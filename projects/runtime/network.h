#pragma once

#include "defines.h"
#include "network_backend.h"
#include "serialization.h"
#include <thread>
#include <mutex>

namespace ryn::net {
	using NetSignature	= uint64_t;
	using NetSession	= uint16_t;
	using NetSequence	= uint16_t;
	using NetMessageType	= uint16_t;
	using NetMessageLength	= uint16_t;

#pragma region Message Structures
#pragma pack(push, 1)
	struct NetMessageHeader : ryn::serialization::AutoSerializable<NetMessageHeader> {
		NetSignature HMAC;
		NetSession SessionID;
		NetSequence Sequence;
	};

	struct NetMessage : ryn::serialization::ISerializable {
		NetMessageType Type;
		NetMessageLength Length;	// Data Length in Bytes
		uint8_t Data[];				// Data (flexible-size)

		size_t serialize(uint8_t* buffer, size_t bufferSize, size_t bufferOffset) const override;
		size_t deserialize(const uint8_t* buffer, size_t bufferSize, size_t bufferOffset) override;
	};
#pragma pack(pop)
#pragma endregion

	class NetServer {
	public:
		struct Config {
			// Frequency, Capacity, etc.
			const uint16_t TickRate;
			const NetSession MaxClients;
			// Network settings
			const uint16_t PortOutboundTraffic;		// Dedicated port for outbound messages
			const uint16_t PortInboundTraffic;		// Dedicated port for inbound messages
			const uint16_t ExpectedUploadMbps;
			const uint16_t ExpectedDownloadMbps;
			// Allocation
			const uint8_t RequestLimitPCPT;			// Request limit per client per tick
			const uint16_t OutboundBandwidthPCPT;	// Bandwidth allocated for outbound messages in bytes per client per tick
			const uint16_t InboundBandwidthPCPT;	// Bandwidth allocated for inbound messages in bytes per client per tick
		};

		struct ClientState {
			const NetSession SessionID;
			const uint64_t Secret[4];			// 256bit unique shared secret key generated on login
			uint8_t RequestsTT;					// Requests send this tick
			uint16_t InboundBandwidthUsageTT;	// Inbound bandwidth used this tick
		};

		NetServer(const NetServer::Config config);
		~NetServer() = default;

		void start();
		void tickStart(float deltaTime);
		void tickEnd();
		void shutdown();

	private:
		NetServer::Config config;

		std::vector<ClientState> clientStates;
		std::unordered_map<backend::NetAddress, size_t, backend::NetAddress::Hasher> clientIndexMap;

		// Sockets
		backend::UDPSocket inSocket;
		backend::UDPSocket outSocket;

		// Threads
		bool shutdownThreads = false;	// Breaks loop inside threads when set to true
		std::thread inThread;
		std::thread outThread;
		std::mutex inMutex;
		std::mutex outMutex;

		// Buffers
		size_t inBufferSize = 0;
		uint8_t* inFrontBuffer = nullptr;
		uint8_t* inBackBuffer = nullptr;

		size_t outBufferSize = 0;
		uint8_t* outFrontBuffer = nullptr;
		uint8_t* outBackBuffer = nullptr;

		// Inbound packet handling
		size_t inBackHead = 0;
		std::condition_variable inBackBufferReady;

		// Outbound packet handling
		// ...

		void createBuffers();
		void createSockets();
		void createThreads();

		void destroyBuffers();
		void destroyThreads();

		void handleInboundPackets();
		void handleOutboundPackets();

		void swapInboundBuffers();
		void swapOutboundBuffers();
	};
}