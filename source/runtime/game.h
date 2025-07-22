#pragma once

// TODO: add member for window
 
// TODO: wrap entt registry
// TODO: wrap entt entitykey
// TODO: wrap entt view

// TODO: create system base class
// TODO: create component "reflection" macro, some inlined code that statically registers all components

// TODO: add ecs registry
// TODO: add ecs systems container (maybe a vector?)
		
// TODO: 
// - implement input management as (ecs) system
// - implement rendering as ecs system
// - implement hierarchy as ecs system

#include "defines.h"

namespace ryn {
	class RYN_API Game {
	public:
		Game() = default;
		virtual ~Game() = default;

		bool isRunning();

	protected:
		virtual void onBeforeInit() {};
		virtual void onAfterInit() {};

		virtual void onBeforeUpdate() {};
		virtual void onAfterUpdate() {};;

		virtual void onBeforeRender() {};
		virtual void onAfterRender() {};

		virtual void onBeforeShutdown() {};
		virtual void onAfterShutdown() {};

	private:
		void initialize();
		void update();
		void render();
		void shutdown();

		friend int run(int argc, char* argv[]);
	};
}