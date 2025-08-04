#pragma once

#include "ryn/defines.h"
#include "ryn/core/type.h"
#include <concepts>

namespace ryn {
	class Subsystem;

	template<typename T>
	concept DerivedFromSubsystem = std::derived_from<T, Subsystem>;

	class RYN_API Game {
	public:
		Game();
		virtual ~Game();
		Game(const Game&) = delete;
		Game& operator=(const Game&) = delete;

		template<DerivedFromSubsystem T>
		T* createSubsystem(T* subsystem) {
			auto typeID = getTypeID<T>();
			internal_createSubsystem(typeID, subsystem);
			return subsystem;
		}

		template<DerivedFromSubsystem T>
		bool deleteSubsystem() {
			auto typeID = getTypeID<T>();
			return internal_deleteSubsystem(typeID);
		}

		template<DerivedFromSubsystem T>
		T* getSubsystem() {
			auto typeID = getTypeID<T>();
			auto ptr = internal_getSubsystem(typeID);
			return static_cast<T*>(ptr);
		}

		bool isRunning();
		void terminate();

	protected:
		virtual void onBeforeInit() {}
		virtual void onBeforeUpdate() {}
		virtual void onBeforeRender() {}
		virtual void onBeforeShutdown() {}
		
		virtual void onAfterInit() {}
		virtual void onAfterUpdate() {}
		virtual void onAfterRender() {}
		virtual void onAfterShutdown() {}

	private:
		void internal_createSubsystem(TypeID typeID, Subsystem* subsystem);
		bool internal_deleteSubsystem(TypeID typeID);
		Subsystem* internal_getSubsystem(TypeID typeID);

		void initialize();
		void update();
		void render();
		void shutdown();

		friend int run(int argc, char* argv[]);

	private:
		struct Impl;
		Impl* m_impl;
	};
}