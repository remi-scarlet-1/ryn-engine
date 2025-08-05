/**
 * @file		subsystem.h
 * @brief		Virtual base class for game subsystems.
 * @details		Subsystems encapsulate modular, self-contained functionality (e.g., input, audio, rendering, or atomic ECS systems),
 *              and are managed by ryn::core::Game. Subclassed by consumer-defined systems or engine components.
 * @warning		**Do not delete** @ref Subsystem "subsystem" instances manually. Ownership is handled by the associated ryn::core::Game instance.
 *
 * @author		Konstantinos Lampros
 * @version		1.0.0
 * @date		2025-08-05
 */

#pragma once

#include "ryn/defines.h"
#include "ryn/game.h"

namespace ryn::core {
	/**
	 * @class	Subsystem
	 * @brief	Virtual base class for game subsystems.
	 * @details	Subsystems are registered dynamically via a ryn::core::Game instance and receive lifecycle callbacks
	 *			such as onInitialize(), onUpdate(), onRender(), and onShutdown(). They must be subclassed
	 *			to implement specific behavior.
	 * @note	Subsystem instances are owned by a Game instance at runtime and **should not be deleted** manually.
	 * @see		Game::registerSubsystem, Game::deleteSubsystem
	 */
	class RYN_API Subsystem {
	public:
		/** @brief	Virtual destructor to ensure proper cleanup in derived classes. */
		virtual ~Subsystem() = default;

	protected:
		/** @brief	Called once when the subsystem is initialized. Override to add custom setup logic. */
		virtual void onInitialize() {}
		/** @brief	Called every frame during the update phase. Override to add update logic. */
		virtual void onUpdate() {}
		/** @brief	Called every frame during the render phase. Override to add rendering logic. */
		virtual void onRender() {}
		/** @brief	Called once before subsystem is destroyed. Override to add cleanup logic. */
		virtual void onShutdown() {}

		/**
		 * @brief	Provides access to the owning Game instance.
		 * @return	Reference to the associated Game object.
		 */
		Game& getGame() { return *m_game; }

	private:
		Game* m_game = nullptr; // (DO NOT DELETE)
		friend class Game;
	};
}