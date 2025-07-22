#pragma once

#include "game.h"

namespace ryn {
	extern Game* createGame();

	int run(int argc, char* argv[]) {
		Game* game = createGame();

		game->initialize();
		while (game->isRunning()) {
			game->update();
			game->render();
		}
		game->shutdown();

		delete game;
		return 0;
	}
}

#ifndef RYN_ENGINE
int main(int argc, char* argv[]) {
	return ryn::run(argc, argv);
}
#endif