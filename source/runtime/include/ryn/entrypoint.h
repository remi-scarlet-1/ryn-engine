/**
 * @file		entrypoint.h
 * @brief		Defines main and entrypoint logic for consuming applications.
 *				Delegates control to an instance of `ryn::Game` which implements the main loop.
 *
 * @author		Konstantinos Lampros
 * @version		1.0.0
 * @date		2025-07-22
 */
#pragma once

#include "ryn/core/game.h"

namespace ryn {
	/**
	 * @brief	Factory function to be implemented by consuming applications. Must return
	 *			a pointer to a custom subclass of `ryn::Game`, which provides the consuming
	 *			application with main loop hooks through overrides.
	 * @return	Pointer to a heap-allocated instance of a class derived from 'ryn::Game'.
	 */
	extern Game* createGame();
	 
	/**
	 * @brief	Internal entrypoint for runtime. Invokes game initialization, update and render
	 *			loops as well as shutdown logic. Called directly from the consuming application's 
	 *			main function.
	 * @param	argc: Number of command-line arguments.
	 * @param	argv: Array of command-line arguments as C-style strings.
	 * @return	Exit code returned to the operating system.
	 */
	int run(int argc, char* argv[]) {
		Game* game = createGame();

		game->initialize();
		while (game->isRunning()) {
			game->update();
			game->render();
		}
		game->shutdown();

		delete game;
		return 0;
	}
}

#ifndef RYN_RUNTIME
/**
 * @brief	Hidden main function definition for consuming applications. Excluded
 *			when building the runtime as a library. Delegates control directly
 *			to `ryn::run()` to start the runtime.
 * @param	argc: Number of command-line arguments.
 * @param	argv: Array of command-line arguments as C-style strings.
 * @return	Exit code returned to the operating system.
 */
int main(int argc, char* argv[]) {
	return ryn::run(argc, argv);
}
#endif