#include "game.h"

#include <iostream>

bool ryn::Game::isRunning()
{
	return true; // TODO: implement actual flag
}

void ryn::Game::initialize()
{
	onBeforeInit();
	std::cout << "Engine initializing...\n";
	onAfterInit();
}

void ryn::Game::update()
{
	onBeforeUpdate();
	std::cout << "Engine updating...\n";
	onAfterUpdate();
}

void ryn::Game::render()
{
	onBeforeRender();
	std::cout << "Engine rendering...\n";
	onAfterRender();
}

void ryn::Game::shutdown()
{
	onBeforeShutdown();
	std::cout << "Engine shutdown...\n";
	onAfterShutdown();
}

