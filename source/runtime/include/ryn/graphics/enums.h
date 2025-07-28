#pragma once

#include "ryn/defines.h"

namespace ryn {
	enum RYN_API GraphicsAPI {
		None = 0,
		OpenGL,
	};

	enum RYN_API VsyncMode {
		Off = 0,
		EveryVBlank,
		EverySecondVBlank,
	};
}