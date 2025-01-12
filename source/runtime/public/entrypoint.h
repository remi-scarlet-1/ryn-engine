/**
* @file    entrypoint.h
* @brief   Defines the engine's entry point and facilitates custom user logic integration with the main loop.
*		   It provides the necessary main function for engine startup, hiding the details from the user.
* @author  Konstantinos Lampros
* @version 1.0.0
* @date    2025-01-12
*/

#pragma once

#include "defines.h"
#include <stdexcept>

namespace ryn {
	/**
	* @brief Forward declaration of the Game class.
	*        The full definition of the Game class is available in 'ryntime.h'. 
	*        Users can inherit from this class to implement custom logic, such as functions 
	*        like 'onStartup', 'onUpdate', and others that integrate with the engine's main loop.
	*/
	class RYN_API Game;

	/**
	* @brief Creates a Game instance.
	*		 The user must define this function with its fixed signature 'ryn::Game* createGame()'.
	*		 It must return a pointer to a new object inheriting from the Game class.
	* @return A pointer to the created 'Game' object.
	* @warning The user must define this function; otherwise, engine initialization will fail.
	*/
	extern Game* createGame();

	/**
	* @brief Runs the main game loop.
	*        This function contains the engine's internal main loop logic and integrates
	*        user-defined logic (e.g., 'onStart', 'onUpdate', etc.) at appropriate points.
	* @param game A pointer to the 'Game' object that contains the user-defined logic.
	* @see createGame
	*/
	RYN_API void run(Game* game);
}

#ifndef RYN_ENGINE
/**
 * @brief The entry point for the user's project and engine setup.
 *        This function calls the user-defined 'createGame' function to create a game instance,
 *		  then starts the engine by running the main game loop.
 *        It is automatically included as the main function in the user's project when 'RYN_ENGINE' is not defined, 
 *        providing a simple entry point to launch the engine.
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return 0 if the application runs successfully, non-zero if there is an error.
 * @throws std::runtime_error If the game creation fails (when 'createGame' returns nullptr).
 */
int main(int argc, char** argv) {
	ryn::Game* game = ryn::createGame();
	if (!game)
		throw new std::runtime_error(
			"Failed to create a game instance. Make sure you have defined the 'createGame' function in the translation unit "
			"where 'entrypoint.h' is included. This function must return a pointer to an object that inherits from 'ryn::Game'. "
			"The required signature is: 'ryn::Game* createGame()'."
		);

	ryn::run(game);
	return 0;
}
#endif // !RYN_ENGINE