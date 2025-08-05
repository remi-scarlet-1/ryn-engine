 /**
  * @file		entrypoint.h
  * @brief		Contains engine bootstrap logic for consumers.
  *	@details	Defines main for consumers which delegates control to ryn::entrypoint::run.
  *				It in turn creates a ryn::Game instance through the ryn::entrypoint::createGame
  *				factory function implemented by the consumer. The instance of ryn::Game is then
  *				used by ryn::entrypoint::run to drive the main engine loop.
  * @warning	Do not include this header multiple times! It defines main.
  *
  * @author		Konstantinos Lampros
  * @version	1.0.0
  * @date		2025-08-05
  */
#pragma once

#include "ryn/core/game.h"

 /**
  * @namespace	ryn::entrypoint
  * @brief		Contains engine bootstrap logic and main loop.
  */
namespace ryn::entrypoint {
	 /**
	  * @brief		Game factory function implemented by the consumer.
	  * @details	Consumer is expected to inherit from the virtual 
	  *				class ryn::Game and implement this factory function
	  *				such that it returns a heap-allocated and type-erased
	  *				instance of their ryn::Game subclass by pointer.
	  * @return		Heap-allocated ryn::Game instance.
	  * @attention	Must be implemented by the consuming application!
	  */
	extern Game* createGame();

	 /**
	  * @brief		Internal runtime entrypoint. Contains main loop.
	  * @details	Creates game instance via ryn::entrypoint::createGame.
	  *				Runs init, update, render, shutdown loops until game signals stop.
	  *				Manages game instance lifetime.
	  * @param		argc Command-line argument count
	  * @param		argv Command-line argument array
	  */
	void run(int argc, char* argv[]) {
		Game* game = createGame();

		game->initialize();
		while (game->isRunning()) {
			game->update();
			game->render();
		}
		game->shutdown();

		delete game;
	}
}

#ifndef RYN_RUNTIME
 /**
  * @brief		Hidden main function implementation for consumers.
  * @details	Delegates control to ryn::entrypoint::run. Not included in runtime library build.
  * @param		argc Command-line argument count
  * @param		argv Command-line argument array
  */
int main(int argc, char* argv[]) {
	ryn::entrypoint::run(argc, argv);
}
#endif