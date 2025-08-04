#include <ryn/entrypoint.h>
#include <ryn/runtime.h>
#include <iostream>

// Defining a game and injecting code into the main loop:
class ExampleGame : public ryn::Game {
protected:
	void onBeforeInit() override { std::cout << "Game initializing..." << std::endl; }
	//void onBeforeUpdate() override;
	//void onBeforeRender() override;
	//void onBeforeShutdown() override;
	//void onAfterInit() override;
	//void onAfterUpdate() override;
	//void onAfterRender() override;
	void onAfterShutdown() override { std::cout << "Game shutdown." << std::endl; }
};

// Defining a custom subsystem and making use of its overrides:
class ExampleSubsystem : public ryn::Subsystem {
protected:
	void initialize() override { std::cout << "Initializing example subsystem!" << std::endl; }
	//void update() override;
	//void render() override;
	void shutdown() override { std::cout << "Shutting down example subsystem..." << std::endl; }
};

// Required by entrypoint.h for initialization
ryn::Game* ryn::createGame() {
	const WindowSettings windowSettings = {
		.title = "Space Invaders",
		.resizable = false,
		.width = 1440,
		.height = 1080,
		.graphicsAPI = OpenGL,
		.vsyncMode = EveryVBlank,
	};

	auto* game = new ExampleGame();

	// Create and register your own subsystems or ones provided by the engine!
	// Once created, they assume a singleton state.
	game->createSubsystem(new Window(windowSettings));
	game->createSubsystem(new ExampleSubsystem());

	// Retrieve existing subsystem singletons by their type. If no subsystem of that
	// type has been created this method returns nullptr.
	auto exampleSubsystemPtr = game->getSubsystem<Window>();

	// You can also delete subsystem singletons like this. Should no singleton exist
	// for the type you are trying to delete the method simply returns false.
	auto deletionSuccess = game->deleteSubsystem<ExampleSubsystem>();

	// Once you return your game instance they main loop will automatically start.
	// It initializes, updates, renders and shuts-down all subsystems created in order
	// of their creation.
	return game;
}