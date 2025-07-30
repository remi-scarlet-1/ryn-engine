#include "ryn/core/game.h"
#include <vector>
#include <optional>

struct ryn::Game::Impl {
	std::optional<WindowSettings> windowInitSettings;

	SharedPtr<Window> window;
	std::vector<UniquePtr<Subsystem>> subsystems;
};

ryn::Game::Game(const WindowSettings& windowSettings) : impl(UniquePtr(new Impl)) { impl->windowInitSettings.emplace(windowSettings); }
ryn::Game::~Game() = default;

ryn::OptionalPtr<ryn::Subsystem> ryn::Game::addSubsystem(UniquePtr<Subsystem> newSubsystem) {
	auto subsystemOptPtr = OptionalPtr<Subsystem>(newSubsystem.get());
	impl->subsystems.push_back(std::move(newSubsystem));
	return subsystemOptPtr;
}

bool ryn::Game::tryDeleteSubsystem(OptionalPtr<Subsystem> subsystemOptPtr) {
	auto [ok, rawPtr] = subsystemOptPtr.tryGet();
	if (!ok)
		return false;

	auto it = std::find_if(impl->subsystems.begin(), impl->subsystems.end(),
		[rawPtr](const UniquePtr<Subsystem>& ptr) {
			return ptr.get() == rawPtr;
		});
	if (it == impl->subsystems.end())
		return false;

	impl->subsystems.erase(it);
	return true;
}

bool ryn::Game::isRunning() {
	return !impl->window->isClosing();
}

void ryn::Game::initialize() {
	onBeforeInit();

	Window::GLFWBackend::initialize();
	impl->window = SharedPtr(new Window(*impl->windowInitSettings));
	impl->windowInitSettings.reset();
	impl->subsystems.reserve(32);
	for (auto& subsystem : impl->subsystems)
		subsystem->initialize();

	onAfterInit();
}

void ryn::Game::update() {
	onBeforeUpdate();

	Window::GLFWBackend::pollEvents();
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

