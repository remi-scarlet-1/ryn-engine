/**
* @file    game.h
* @brief   Declares the 'Game' class, which serves as the base class for user-defined main loop logic.
*          This class provides virtual methods for hooking into the game loop. Users can inherit from this class
*          and override its methods to implement custom behavior for their games.
* @author  Konstantinos Lampros
* @version 1.0.0
* @date    2025-01-12
*/

#pragma once

#include "public/defines.h"

namespace ryn {
	/**
	* @class Game
	* @brief Base class for hooking into the engine's main loop.
	*
	* The 'Game' class allows users to define custom behavior for the engine's main loop.
	* Users must inherit from this class and override its methods to provide their own logic
	* for e.g. startup, updating, shutdown, etc. which will be slotted into the engine's main loop.
	* This class does not manage the game loop directly, but serves as a way to hook user code into it.
	*/
	class RYN_API Game {
	public:
		virtual ~Game() = default;

		/**
		* @brief Callback for successful engine start.
		*        This method is called after the engine was successfully started.
		*/
		virtual void onStarted() = 0;

		/**
		* @brief Callback for engine update.
		*        This method is called at the start of each update cycle in the engine's main loop.
		*/
		virtual void onUpdate() = 0;

		/**
		* @brief Callback for engine shutdown.
		*        This method is called when the engine is shutting down.
		*/
		virtual void onShutdown() = 0;
	};
}