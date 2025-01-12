#pragma once

#include "defines.h"
#include <stdexcept>

namespace ryn {
	// Forward declare Game, because classes should only be defined for the user in ryntime.h
	class RYN_API Game;

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