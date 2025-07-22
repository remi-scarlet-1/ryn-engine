#include <ryn/entrypoint.h>
#include <ryn/runtime.h>

class ExampleGame : public ryn::Game {
public:
	ExampleGame(const ryn::WindowSettings& settings) : ryn::Game(settings) {}

protected:
	// Example callbacks to hook into:
	void onBeforeInit() override {};
	void onAfterInit() override {};

	void onBeforeUpdate() override {};
	void onAfterUpdate() override {};

	void onBeforeRender() override {};
	void onAfterRender() override {};

	void onBeforeShutdown() override {};
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

	return new ExampleGame(windowSettings);
}