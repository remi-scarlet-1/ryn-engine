#pragma once

#include "defines.h"
#include <stdexcept>

namespace ryn {
	class RYN_API Game {
	public:
		virtual ~Game() = default;

		virtual void onStarted() = 0;
		virtual void onUpdate() = 0;
		virtual void onShutdown() = 0;
	};

	extern Game* createGame();
	RYN_API void run(Game* game);
}

#ifndef RYN_ENGINE
int main(int argc, char** argv) {
	ryn::Game* game = ryn::createGame();
	if (!game)
		throw new std::runtime_error("Failed to create application");

	ryn::run(game);
	return 0;
}
#endif // !RYN_ENGINE