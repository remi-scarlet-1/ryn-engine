#pragma once

// TODO: 
// - [ ] wrap entt registry
// - [ ] wrap entt entity key
// - [ ] wrap entt view
//
// - [ ] create system base class
// - [ ] create component "reflection" macro, some in-lined code that statically registers all components
//
// - [ ] add ECS registry
// - [ ] add ECS systems container (maybe a vector?)
// 
// - [ ] implement input management as (ECS) system
// - [ ] implement rendering as ECS system
// - [ ] implement hierarchy as ECS system

#include "ryn/defines.h"
#include "ryn/core/window.h"

namespace ryn {
	class RYN_API Game {
	public:
		Game(const WindowSettings& windowSettings);
		virtual ~Game();

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

	private:
		Window* window;
	};
}