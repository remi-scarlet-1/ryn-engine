#pragma once

#include "ryn/defines.h"
#include "ryn/graphics/window.h"
#include "ryn/core/subsystem.h"

namespace ryn {
	class RYN_API OpenGLRenderer : public Subsystem {
	public:
		// TODO: replace dependency injection with SharedPtr
		OpenGLRenderer(Window& window);
		~OpenGLRenderer() = default;

		// TODO: implement initialize
		//void initialize() override;

		// TODO: implement shutdown
		//void shutdown() override;

	private:

	};
}