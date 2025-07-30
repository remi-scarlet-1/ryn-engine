#pragma once

#include "ryn/defines.h"
#include "ryn/graphics/window.h"
#include "ryn/core/subsystem.h"

namespace ryn {
	class RYN_API OpenGLRenderer : public Subsystem {
	public:
		OpenGLRenderer(SharedPtr<Window> window);
		~OpenGLRenderer() = default;

		// TODO: implement initialize
		//void initialize() override;

		// TODO: implement shutdown
		//void shutdown() override;

	private:
		SharedPtr<Window> window;
	};
}