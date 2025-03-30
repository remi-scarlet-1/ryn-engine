#include "network.h"
#include <format>

using namespace ryn::net;

NetServer::NetServer(const NetServer::Config config) 
	  :	config(config)
{ }

void NetServer::start() {
	// Calculate buffer sizes
	inBufferSize = config.MaxClients * config.InboundBandwidthPCPT;
	outBufferSize = config.MaxClients * config.OutboundBandwidthPCPT;

	// Ensure sufficient upload & download speeds given server config
	if (config.ExpectedDownloadMbps * 1'000'000 / 8 < inBufferSize * config.TickRate)
		throw std::runtime_error(std::format(
			"ExpectedDownloadMbps aren't sufficient to sustain {} max clients at {} ticks per second.",
			config.MaxClients, config.TickRate));

	if (config.ExpectedUploadMbps * 1'000'000 / 8 < outBufferSize * config.TickRate)
		throw std::runtime_error(std::format(
			"ExpectedUploadMbps aren't sufficient to sustain {} max clients at {} ticks per second.",
			config.MaxClients, config.TickRate));

	createBuffers();
	createSockets();
	createThreads();
}

void NetServer::shutdown() {
	destroyThreads();
	destroyBuffers();
}

void NetServer::createBuffers() {
	inFrontBuffer = (uint8_t*)malloc(inBufferSize);
	inBackBuffer = (uint8_t*)malloc(inBufferSize);
	outFrontBuffer = (uint8_t*)malloc(outBufferSize);
	outBackBuffer = (uint8_t*)malloc(outBufferSize);

	// Ensure allocation was successful
	if (!inFrontBuffer || !inBackBuffer || !outFrontBuffer || !outBackBuffer || inBufferSize == 0 || outBufferSize == 0) {
		destroyBuffers();
		throw std::bad_alloc();
	}
}

void NetServer::createSockets() {
	inSocket = backend::UDPSocket(config.PortInboundTraffic, false);
	outSocket = backend::UDPSocket(config.PortOutboundTraffic, false);
}

void NetServer::createThreads() {
	try {
		inThread = std::thread(&NetServer::handleInboundPackets, this);
		outThread = std::thread(&NetServer::handleOutboundPackets, this);
	}
	catch (const std::exception& ex) {
		throw std::runtime_error(std::format("Failed starting the network I/O threads. Details: \n {}", ex.what()));
	}
}

void NetServer::destroyBuffers() {
	if (inFrontBuffer)	{ free(inFrontBuffer);	inFrontBuffer = nullptr;  }
	if (inBackBuffer)	{ free(inBackBuffer);	inBackBuffer = nullptr;	  }
	if (outFrontBuffer) { free(outFrontBuffer); outFrontBuffer = nullptr; }
	if (outBackBuffer)	{ free(outBackBuffer);	outBackBuffer = nullptr;  }
	inBufferSize = 0;
	inBackHead = 0;
	outBufferSize = 0;
}

void NetServer::destroyThreads() {
	shutdownThreads = true;
	inBackBufferReady.notify_one(); // Wake inbound thread (so it can be joined)
	inThread.join();
	outThread.join();
}

void NetServer::handleInboundPackets() {
	while (!shutdownThreads) {
		// Ensure main thread doesn't attempt modifying back buffer state during this iteration
		std::unique_lock<std::mutex> lock(inMutex);

		// Sleep thread if 'inBackBuffer' memory runs out
		// Wake thread if memory is available agains OR threads are shutting down
		// Mutex is released temporarily while waiting
		inBackBufferReady.wait(lock, [this]() { return inBackHead < inBufferSize || shutdownThreads; });
		if (shutdownThreads) break;

		// Attempt to receive new package and its sender information
		uint8_t* bufferHead = inBackBuffer + inBackHead;
		size_t bufferSize = inBufferSize - inBackHead;
		backend::NetAddress sender;

		int bytesReceived = inSocket.receive(bufferHead, bufferSize, &sender);
		if (bytesReceived <= 0) 
			continue;

		// Ensure sender has an entry in 'clientIndexMap' (sender is a logged-in client)
		auto it = clientIndexMap.find(sender);
		if (it == clientIndexMap.end()) 
			continue;

		// Get client state
		ClientState& client = clientStates[it->second];

		// Ensure client hasn't exceeded request and bandwidth limits for the current tick
		if (client.RequestsTT >= config.RequestLimitPCPT) 
			continue;
		if (client.InboundBandwidthUsageTT + bytesReceived >= config.InboundBandwidthPCPT) 
			continue;

		// Update client and buffer state
		client.RequestsTT++;
		client.InboundBandwidthUsageTT += bytesReceived;
		inBackHead += bytesReceived;

		// TODO: processing of package (HMAC?, Indexing?)
	}
}

void NetServer::handleOutboundPackets() {
	while (!shutdownThreads) {
		// Ensure main thread doesn't attempt modifying back buffer state during this iteration
		std::unique_lock<std::mutex> lock(outMutex);

		// TODO: Wait if buffer has been worked off
		// TODO: Send off all pending outbound messages from 'outBackBuffer'
	}
}

void NetServer::swapInboundBuffers() {
	// (Lock mutex) Wait for chance to swap out front/back buffers without causing race conditions
	std::lock_guard<std::mutex> lock(inMutex);
	std::swap(inFrontBuffer, inBackBuffer);

	inBackHead = 0; // Reset write head to overwrite old data in the new 'inBackBuffer'
	inBackBufferReady.notify_one(); // Wake inbound I/O thread so it can continue processing
}

void NetServer::swapOutboundBuffers() {
	// (Lock mutex) Wait for chance to swap out front/back buffers without causing race conditions
	std::lock_guard<std::mutex> lock(outMutex);
	std::swap(outFrontBuffer, outBackBuffer);
}