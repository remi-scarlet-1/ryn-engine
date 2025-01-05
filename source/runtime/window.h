#pragma once

#include "public/defines.h"

struct GLFWwindow;

namespace ryn {
	struct WindowSettings {
		const char* title;
		const bool resizable;
		int width, height;
		bool vsync;
	};

	class Window {
	public:
		Window(WindowSettings settings);
		~Window();

		// Hack: disable copying of windows, so the destructor isn't accidentaly called
		Window(const Window&) = delete;            // Disable copy constructor
		Window& operator=(const Window&) = delete; // Disable copy assignment

		static void pollEvents();
		GLFWwindow* getGlfwHandle();
		void* getNativeHandle();
		bool getShouldClose();

	private:
		GLFWwindow* glfwHandle;
	};
}