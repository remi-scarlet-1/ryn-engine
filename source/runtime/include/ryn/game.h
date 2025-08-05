/**
 * @file		game.h
 * @brief		Virtual base class for consuming applications. 
 * @details		Defines ryn::core::Game which acts as the base class for consuming applications.
 *				It provides virtual hooks for code injection into the main loop, management of ryn::core::Subsystem instances and
 *				application lifetime control.
 * @attention	Consuming applications **are expected** to subclass ryn::core::Game and provide a heap-allocated
 *				instance of their subclass via ryn::entrypoint::createGame. See its documentation for implementation details.
 * 
 * @author		Konstantinos Lampros
 * @version		1.0.0
 * @date		2025-08-05
 */

#pragma once

#include "ryn/defines.h"
#include "ryn/meta.h"
#include <concepts>

// Forward declarations
namespace ryn {
	namespace core {
		class Subsystem;
	}
	namespace entrypoint {
		void run(int argc, char* argv[]);
	}
}

namespace ryn::core {
	/**
	 * @concept		DerivedFromSubsystem
	 * @brief		Concept constraint. Requires T to inherit from ryn::Subsystem.
	 */
	template<typename T>
	concept DerivedFromSubsystem = std::derived_from<T, Subsystem>;

	/**
	 * @class		Game
	 * @brief		Virtual base class for consuming applications.
	 * @details		Provides virtual hooks for code injection into the main loop, management of ryn::core::Subsystem instances and
	 *				application lifetime control.
	 * @note		Active ryn::core::Game instance is owned and controlled by runtime during execution.
	 */
	class RYN_API Game {
	public:
		Game();
		virtual ~Game();

		/** @brief Deleted copy constructor. */
		Game(const Game&) = delete;
		/** @brief Deleted copy assignment operator. */
		Game& operator=(const Game&) = delete;

		/**
		 * @brief		Register a subsystem.
		 * @details		Registers the given @ref Subsystem "subsystem" instance by type. If successful, it assumes a singleton state.
		 * @tparam		T Subsystem type, must inherit from ryn::core::Subsystem.
		 * @param		subsystem Pointer to heap-allocated subsystem. (Game instance takes over ownership)
		 * @warning		Subsystems of the same type **cannot be registered twice**. Attemption to do so has no effect.
		 */
		template<DerivedFromSubsystem T>
		void registerSubsystem(T* subsystem) {
			auto typeID = meta::getTypeID<T>();
			internal_registerSubsystem(typeID, subsystem);
		}

		/**
		 * @brief		Delete a registered subsystem by type.
		 * @details		Attempts to find the types corresponding @ref Subsystem "subsystem" singleton. If found, delete is called on it.
		 * @tparam		T Subsystem type, must inherit from ryn::core::Subsystem.
		 * @return		true if subsystem was found and deleted.
		 */
		template<DerivedFromSubsystem T>
		bool deleteSubsystem() {
			auto typeID = meta::getTypeID<T>();
			return internal_deleteSubsystem(typeID);
		}

		/**
		 * @brief		Get a registered subsystem by type.
		 * @details		Attempts to find the types corresponding @ref Subsystem "subsystem" singleton. If found, it gets returned.
		 * @tparam		T Subsystem type, must inherit from ryn::core::Subsystem.
		 * @return		Pointer to subsystem or nullptr.
		 * @warning		This method might **return a nullptr** should no corresponding subsystem singleton be found!
		 */
		template<DerivedFromSubsystem T>
		T* getSubsystem() {
			auto typeID = meta::getTypeID<T>();
			auto ptr = internal_getSubsystem(typeID);
			return static_cast<T*>(ptr);
		}

		/**
		 * @brief		Check if app is running.
		 * @return		true if app is running.
		 */
		bool isRunning();

		/**
		 * @brief		Request termination.
		 * @details		Signals main loop to stop.
		 */
		void terminate();

	protected:
		/** @brief	Main loop hook before init phase. */
		virtual void onBeforeInit() {}
		/** @brief	Main loop hook before update phase. */
		virtual void onBeforeUpdate() {}
		/** @brief	Main loop hook before render phase. */
		virtual void onBeforeRender() {}
		/** @brief	Main loop hook before shutdown phase. */
		virtual void onBeforeShutdown() {}
		/** @brief	Main loop hook after init phase. */
		virtual void onAfterInit() {}
		/** @brief	Main loop hook after update phase. */
		virtual void onAfterUpdate() {}
		/** @brief	Main loop hook after render phase. */
		virtual void onAfterRender() {}
		/** @brief	Main loop hook after shutdown phase. */
		virtual void onAfterShutdown() {}

	private:
		void internal_registerSubsystem(meta::TypeID typeID, Subsystem* subsystem);
		bool internal_deleteSubsystem(meta::TypeID typeID);
		Subsystem* internal_getSubsystem(meta::TypeID typeID);

		void initialize();
		void update();
		void render();
		void shutdown();

		friend void ryn::entrypoint::run(int argc, char* argv[]);

	private:
		struct Impl;
		Impl* m_impl;
	};
}