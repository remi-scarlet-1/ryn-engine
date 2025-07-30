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

bool ryn::Game::isRunning() {
	return !impl->window->isClosing();
}

ryn::SharedPtr<ryn::Window> ryn::Game::getWindow() {
	return impl->window;
}

ryn::OptionalPtr<ryn::Subsystem> ryn::Game::addSubsystem(UniquePtr<Subsystem> newSubsystem) {
	auto subsystemOptional = OptionalPtr<Subsystem>(newSubsystem.get());
	impl->subsystems.push_back(std::move(newSubsystem));
	return subsystemOptional;
}

bool ryn::Game::tryDeleteSubsystem(OptionalPtr<Subsystem> subsystemOptional) {
	auto [ok, ptr] = subsystemOptional.tryGet();
	if (!ok)
		return false;

	auto it = std::find_if(impl->subsystems.begin(), impl->subsystems.end(),
		[ptr](const UniquePtr<Subsystem>& itPtr) {
			return itPtr.get() == ptr;
		});
	if (it == impl->subsystems.end())
		return false;

	impl->subsystems.erase(it);
	return true;
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

