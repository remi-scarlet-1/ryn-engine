/**
 * @file		game.h
 * @brief		Contains base class for consumer applications. Manages game subsystems and main loop.
 * @details		Consumer inherits from ryn::Game and can implement virtual hooks for engine loop.
 *				Provides runtime subsystem management and application lifetime control.
 *
 * @author		Konstantinos Lampros
 * @version		1.0.0
 * @date		2025-08-05
 */

#pragma once

#include "ryn/defines.h"
#include "ryn/core/meta.h"
#include <concepts>

namespace ryn {
	// Forward declarations
	class Subsystem;
	namespace entrypoint {
		void run(int argc, char* argv[]);
	}

	/**
	 * @concept	DerivedFromSubsystem
	 * @brief	Concept constraint. Requires T to inherit from ryn::Subsystem.
	 */
	template<typename T>
	concept DerivedFromSubsystem = std::derived_from<T, Subsystem>;

	/**
	 * @class	Game
	 * @brief	Base class for consumer applications. Manages game subsystems and drives engine lifecycle.
	 * @details	Consumer subclass can provide logic via virtual hooks. Manages game subsystems dynamically.
	 *			Game instance is owned and controlled by runtime during execution.
	 */
	class RYN_API Game {
	public:
		Game();
		virtual ~Game();
		Game(const Game&) = delete;
		Game& operator=(const Game&) = delete;

		/**
		 * @brief	Create and register a subsystem.
		 * @details	Registers the given subsystem instance by type. Runtime takes over ownership.    
		 *			Subsystem instance takes on singleton state for its type.
		 * @tparam	T Subsystem type, must inherit from ryn::Subsystem.
		 * @param	subsystem Pointer to heap-allocated subsystem instance.
		 * @warning	Subsystem of the same type cannot be registered twice. Second call is ignored.
		 */
		template<DerivedFromSubsystem T>
		void createSubsystem(T* subsystem) {
			auto typeID = meta::getTypeID<T>();
			internal_createSubsystem(typeID, subsystem);
		}

		/**
		 * @brief	Delete a registered subsystem by type.
		 * @details	Attempts to remove subsystem matching the given type.
		 *			Calls delete on the singleton instance if found.
		 * @tparam	T Subsystem type, must inherit from ryn::Subsystem.
		 * @return	True if subsystem was found and deleted.
		 */
		template<DerivedFromSubsystem T>
		bool deleteSubsystem() {
			auto typeID = meta::getTypeID<T>();
			return internal_deleteSubsystem(typeID);
		}

		/**
		 * @brief	Get a registered subsystem by type.
		 * @details	Returns pointer to registered subsystem singleton if found.
		 * @tparam	T Subsystem type, must inherit from ryn::Subsystem.
		 * @return	Pointer to subsystem or nullptr.
		 */
		template<DerivedFromSubsystem T>
		T* getSubsystem() {
			auto typeID = meta::getTypeID<T>();
			auto ptr = internal_getSubsystem(typeID);
			return static_cast<T*>(ptr);
		}

		/**
		 * @brief	Check if app is running.
		 * @return	True if app is running.
		 */
		bool isRunning();

		/**
		 * @brief	Request termination.
		 * @details	Signals main loop to stop.
		 */
		void terminate();

	protected:
		/** @brief	Hook before init phase. */
		virtual void onBeforeInit() {}
		/** @brief	Hook before update phase. */
		virtual void onBeforeUpdate() {}
		/** @brief	Hook before render phase. */
		virtual void onBeforeRender() {}
		/** @brief	Hook before shutdown phase. */
		virtual void onBeforeShutdown() {}
		/** @brief	Hook after init phase. */
		virtual void onAfterInit() {}
		/** @brief	Hook after update phase. */
		virtual void onAfterUpdate() {}
		/** @brief	Hook after render phase. */
		virtual void onAfterRender() {}
		/** @brief	Hook after shutdown phase. */
		virtual void onAfterShutdown() {}

	private:
		void internal_createSubsystem(meta::TypeID typeID, Subsystem* subsystem);
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