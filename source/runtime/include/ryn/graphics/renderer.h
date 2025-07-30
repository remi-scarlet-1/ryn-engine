#pragma once

#include "ryn/defines.h"
#include "ryn/graphics/window.h"
#include "ryn/core/subsystem.h"

namespace ryn {
	class RYN_API Renderer : public Subsystem {
	public:
		Renderer(SharedPtr<Window> window);
		~Renderer() = default;

	private:
		SharedPtr<Window> window;
	};
}