#include "ryn/core/game.h"

#include "ryn/core/subsystem.h"
#include <vector>

struct ryn::Game::Impl {
	UniquePtr<Window> window;
	std::vector<UniquePtr<Subsystem>> subsystems;
};

ryn::Game::Game(const WindowSettings& windowSettings)
	: impl(UniquePtr(new Impl { 
		UniquePtr(new Window(windowSettings)),
		{}
	})) 
{	
	impl->subsystems.reserve(32);
}

ryn::Game::~Game() = default;

bool ryn::Game::isRunning() {
	return !impl->window->isClosing();
}

ryn::Subsystem* ryn::Game::addSubsystem(UniquePtr<Subsystem> subsystem) {
	Subsystem* rawPtr = subsystem.get();
	impl->subsystems.push_back(std::move(subsystem));
	return rawPtr;
}

bool ryn::Game::destroySubsystem(Subsystem* subsystem) {
	auto it = std::find_if(impl->subsystems.begin(), impl->subsystems.end(),
		[subsystem](const UniquePtr<Subsystem>& ptr) {
			return ptr.get() == subsystem;
		});

	if (it != impl->subsystems.end()) {
		impl->subsystems.erase(it);
		return true;
	}

	return false;
}

void ryn::Game::initialize() {
	onBeforeInit();

	for (auto& subsystem : impl->subsystems)
		subsystem->initialize();

	onAfterInit();
}

void ryn::Game::update() {
	onBeforeUpdate();

	Window::pollEvents();
	for (auto& subsystem : impl->subsystems)
		subsystem->update();

	onAfterUpdate();
}

void ryn::Game::render() {
	onBeforeRender();

	for (auto& subsystem : impl->subsystems)
		subsystem->render();

	onAfterRender();
}

void ryn::Game::shutdown() {
	onBeforeShutdown();

	for (auto& subsystem : impl->subsystems)
		subsystem->shutdown();

	onAfterShutdown();
}

