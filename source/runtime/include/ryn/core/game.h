#pragma once

#include "ryn/defines.h"
#include "ryn/core/memory.h"
#include "ryn/core/subsystem.h"
#include "ryn/graphics/window.h"

// TODO: Write documentation
namespace ryn {
	class RYN_API Game {
	public:
		Game(const WindowSettings& windowSettings);
		virtual ~Game();
		 
		// TODO: Rethink responsibilities of this class

		bool isRunning();
		SharedPtr<Window> getWindow();
		OptionalPtr<Subsystem> addSubsystem(UniquePtr<Subsystem> newSubsystem);
		bool tryDeleteSubsystem(OptionalPtr<Subsystem> subsystemOptional);

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
		void initialize();
		void update();
		void render();
		void shutdown();

		friend int run(int argc, char* argv[]);

	private:
		struct Impl;
		UniquePtr<Impl> impl;
	};
}