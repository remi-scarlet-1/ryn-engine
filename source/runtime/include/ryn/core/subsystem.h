#pragma once

#include "ryn/defines.h"
#include "ryn/core/game.h"

namespace ryn {
	class RYN_API Subsystem {
	public:
		virtual ~Subsystem() = default;

	protected:
		virtual void initialize() {}
		virtual void update() {}
		virtual void render() {}
		virtual void shutdown() {}

		Game& getGame() { return *m_game; }

	private:
		Game* m_game = nullptr; // non-owning raw pointer (DON'T DELETE!)
		friend class Game;
	};
}