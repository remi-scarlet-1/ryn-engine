#include <ryn.h>
#include <entrypoint.h>

class ExampleGame : public ryn::Game {
protected:
	// Example callbacks to hook into:
	/*
	void onBeforeInit() override {};
	void onAfterInit() override {};

	void onBeforeUpdate() override {};
	void onAfterUpdate() override {};

	void onBeforeRender() override {};
	void onAfterRender() override {};

	void onBeforeShutdown() override {};
	void onAfterShutdown() override {};
	*/
};

ryn::Game* ryn::createGame() {
	return new ExampleGame();
}