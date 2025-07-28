#include <ryn/entrypoint.h>
#include <ryn/runtime.h>
#include <iostream>

// Example injecting custom ecs system
class ExampleSubsystem : public ryn::Subsystem {
public:
	ExampleSubsystem() = default;
	~ExampleSubsystem() = default;

	void initialize() override { std::cout << "Hello, example subsystem!" << std::endl; }
	void update() override {}
	void render() override {}
	void shutdown() override { std::cout << "Example subsystem, out!" << std::endl; }
};

// Example setting up the engine
class ExampleGame : public ryn::Game {
public:
	ExampleGame(const ryn::WindowSettings& settings) : ryn::Game(settings) {}

protected:
	// Example main loop hooks
	void onBeforeInit() override {};
	void onBeforeUpdate() override {};
	void onBeforeRender() override {};
	void onBeforeShutdown() override {};

	void onAfterInit() override {};
	void onAfterUpdate() override {};
	void onAfterRender() override {};
	void onAfterShutdown() override {};
};

ryn::Game* ryn::createGame() {
	WindowSettings windowSettings = {
		.title = "Space Invaders",
		.resizable = false,
		.width = 1200,
		.height = 900,
		.vsync = true,
	};

	auto* game = new ExampleGame(windowSettings);
	game->addSubsystem(ryn::UniquePtr<ryn::Subsystem>(new ExampleSubsystem()));
	return game;
}