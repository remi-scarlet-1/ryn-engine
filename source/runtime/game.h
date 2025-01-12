#pragma once

#include "public/defines.h"

namespace ryn {
	class RYN_API Game {
	public:
		virtual ~Game() = default;

		virtual void onStarted() = 0;
		virtual void onUpdate() = 0;
		virtual void onShutdown() = 0;
	};
}