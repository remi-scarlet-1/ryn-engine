#pragma once

#include "public/defines.h"
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

	class NetServer {
	public:
		struct Config {
			const uint16_t TickRate;
			const NetSession MaxClients;
			const uint8_t RequestLimitPCPT;			// Request limit per client per tick
			const uint16_t OutboundBandwidthPCPT;	// Bandwidth allocated for outbound messages in bytes per client per tick
			const uint16_t InboundBandwidthPCPT;	// Bandwidth allocated for inbound messages in bytes per client per tick
			const uint16_t ExpectedUploadMbps;		// Corresponds to outbound bandwidth/messages
			const uint16_t ExpectedDownloadMbps;	// Corresponds to inbound bandwidth/messages
		};

		struct ClientState {
			const NetSession SessionID;
			const uint64_t Secret[4];			// 256bit unique shared secret key generated on login
			uint8_t RequestsTT;					// Requests send this tick
			uint16_t InboundBandwidthUsageTT;	// Inbound bandwidth used this tick
		};

		NetServer(const NetServer::Config config, const backend::UDPSocket socket);
		~NetServer() = default;

		void start();
		void tickStart(float deltaTime);
		void tickEnd();
		void shutdown();

	private:
		NetServer::Config config;
		backend::UDPSocket socket;

		// Client Tracking
		NetSession connectedClientCount;
		std::unordered_map<backend::NetAddress, ClientState> clientsStates; // TODO: after reworking and finalizing backend::NetAddress consider custom hash function

		// Threading
		bool shutdownIOThreads = false; // Breaks loop inside I/O threads when set to true
		std::thread inThread;
		std::thread outThread;
		std::mutex inMutex;		// Locked by 'inThread' during shared state operations
		std::mutex outMutex;	// Locked by 'outThread' during shared state operations

		// Inbound messages
		uint8_t* inFrontBuffer = nullptr;	// Holds received messages (for current tick)
		uint8_t* inBackBuffer = nullptr;	// Actively receives new messages (for next tick)
		size_t inBufferSize = 0;
		size_t inBackHead = 0;
		std::condition_variable inBackBufferReady;	// Used to wake 'inThread' through .notify_one()

		// Outbound messages
		uint8_t* outFrontBuffer = nullptr;	// Holds queued up outbound messages (from current tick)
		uint8_t* outBackBuffer = nullptr;	// Actively sends off outbound messages (from last ticks queue)
		size_t outBufferSize = 0;

		void handleInboundPackets();
		void handleOutboundPackets();
		void swapInboundBuffers();
		void swapOutboundBuffers();

		// Helpers
		void terminateIOThreads();
		void freeIOBuffers();
	};
}