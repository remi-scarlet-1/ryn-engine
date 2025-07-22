#include "ryn/core/game.h"

#include <iostream>

ryn::Game::Game(const WindowSettings& windowSettings)
{
	window = new Window(windowSettings);
}

ryn::Game::~Game()
{
	delete window;
}

bool ryn::Game::isRunning()
{
	return !window->isClosing();
}

void ryn::Game::initialize()
{
	onBeforeInit();
	// TODO: initialize engine

	onAfterInit();
}

void ryn::Game::update()
{
	onBeforeUpdate();
	// TODO: update engine
	Window::pollEvents();

	onAfterUpdate();
}

void ryn::Game::render()
{
	onBeforeRender();
	// TODO: render engine

	onAfterRender();
}

void ryn::Game::shutdown()
{
	onBeforeShutdown();
	// TODO: shutdown engine

	onAfterShutdown();
}

