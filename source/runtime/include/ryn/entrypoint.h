/**
 * @file		entrypoint.h
 * @brief		Engine bootstrap.
 * @details		Defines the main function for the consuming application, delegating control to ryn::entrypoint::run.
 *				This function creates a type-erased instance of a consumer-defined subclass of ryn::core::Game via ryn::entrypoint::createGame,
 *				which the runtime uses to invoke ```init```, ```update```, ```render``` and ```shutdown``` methods in the main loop.
 * @attention	The consuming application **must implement** the game factory ryn::entrypoint::createGame. See its documentation for implementation details.
 * @warning		Do **not** include this header multiple times; it defines the application's ```main``` function.
 * 
 * @author		Konstantinos Lampros
 * @version		1.0.0
 * @date		2025-08-05
 */

#pragma once

#include "ryn/game.h"

namespace ryn::entrypoint {
	/**
	 * @brief		Game factory. To be implemented by the consuming application.
	 * @attention	This method must be implemented by the **consuming application**!
	 *				It should return a heap-allocated instance of a consumer-defined subclass of ryn::core::Game. 
	 * @return		A heap-allocated, type-erased instance of a consumer-defined subclass of ryn::core::Game.
	 */
	extern core::Game* createGame();

	/**
	 * @brief		Runtime entrypoint and main loop.
	 * @details		Creates a ryn::core::Game instance via ryn::entrypoint::createGame. Uses it to invoke ```init```, ```update```, 
	 *				```render``` and ```shutdown``` methods repeatedly until it requests termination.
	 * @param		argc Command-line argument count
	 * @param		argv Command-line argument array
	 */
	void run(int argc, char* argv[]) {
		core::Game* game = createGame();

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
 * @brief		Hidden main function implementation for consuming applications.
 * @details		Delegates control to ryn::entrypoint::run. Used for convenience.
 * @param		argc Command-line argument count
 * @param		argv Command-line argument array
 */
int main(int argc, char* argv[]) {
	ryn::entrypoint::run(argc, argv);
}
#endif