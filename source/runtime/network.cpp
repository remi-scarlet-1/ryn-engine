#include "network.h"
#include <format>

using namespace ryn::net;

NetServer::NetServer(const NetServer::Config config, const backend::UDPSocket socket) 
	  :	config(config),
		socket(socket)
{ }

void NetServer::start() {
	inBufferSize = config.MaxClients * config.InboundBandwidthPCPT;
	outBufferSize = config.MaxClients * config.OutboundBandwidthPCPT;

	// Ensure sufficient upload & download speeds given configuration
	if (config.ExpectedDownloadMbps * 1'000'000 / 8 < inBufferSize * config.TickRate)
		throw std::runtime_error(std::format(
			"ExpectedDownloadMbps aren't sufficient to sustain {} max clients at {} ticks per second.",
			config.MaxClients, config.TickRate));

	if (config.ExpectedUploadMbps * 1'000'000 / 8 < outBufferSize * config.TickRate)
		throw std::runtime_error(std::format(
			"ExpectedUploadMbps aren't sufficient to sustain {} max clients at {} ticks per second.",
			config.MaxClients, config.TickRate));

	// Allocate I/O buffers
	inFrontBuffer = (uint8_t*)malloc(inBufferSize);
	inBackBuffer = (uint8_t*)malloc(inBufferSize);
	outFrontBuffer = (uint8_t*)malloc(outBufferSize);
	outBackBuffer = (uint8_t*)malloc(outBufferSize);

	// Ensure allocation of I/O buffers was successful
	if (!inFrontBuffer || !inBackBuffer || !outFrontBuffer || !outBackBuffer || inBufferSize == 0 || outBufferSize == 0) {
		freeIOBuffers();
		throw std::bad_alloc();
	}

	// Start I/O threads
	try {
		inThread = std::thread(&NetServer::handleInboundPackets, this);
		outThread = std::thread(&NetServer::handleOutboundPackets, this);
	}
	catch (const std::exception& ex) {
		throw std::runtime_error(std::format("Failed starting the network I/O threads. Details: \n {}", ex.what()));
	}
}

void NetServer::shutdown() {
	// Stop I/O threads & free I/O buffers
	terminateIOThreads();
	freeIOBuffers();
}

void NetServer::handleInboundPackets() {
	while (!shutdownIOThreads) {
		// Ensure main thread doesn't attempt modifying back buffer state during this iteration
		std::unique_lock<std::mutex> lock(inMutex);
		
		// Sleep thread if 'inBackBuffer' memory runs out
		// Wake thread if memory is available agains OR threads are shutting down
		inBackBufferReady.wait(lock, [this]() { return inBackHead < inBufferSize || shutdownIOThreads; });
		if (shutdownIOThreads) break;

		int bytesReceived = socket.receive(inBackBuffer + inBackHead, inBufferSize - inBackHead);
		if (bytesReceived > 0) {
			// TODO: Get connection address of sender
			// TODO: Ensure sender isn't overusing their upload limit per tick ('InboundBandwidthPCPT')
			// TODO: Ensure sended isn't overusing their poll limit per tick ('RequestLimitPCPT')

			// TODO: If package valid, update sender upload/poll usage & increment 'inBackHead'
			inBackHead += bytesReceived;

			// TODO: Potentially index package. Keep a sorted table of each clients packages as pointers to buffer.
		}
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

void NetServer::terminateIOThreads() {
	shutdownIOThreads = true;
	inBackBufferReady.notify_one(); // Wake inbound I/O thread so it can be joined
	inThread.join();
	outThread.join();
}

void NetServer::freeIOBuffers() {
	if (inFrontBuffer)	{ free(inFrontBuffer);	inFrontBuffer = nullptr;  }
	if (inBackBuffer)	{ free(inBackBuffer);	inBackBuffer = nullptr;	  }
	if (outFrontBuffer) { free(outFrontBuffer); outFrontBuffer = nullptr; }
	if (outBackBuffer)	{ free(outBackBuffer);	outBackBuffer = nullptr;  }
	inBufferSize = 0;
	inBackHead = 0;
	outBufferSize = 0;
}
