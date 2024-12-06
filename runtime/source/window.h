#pragma once

#include <GLFW/glfw3.h>

namespace rn {
	class Window {
	public:
		Window(const char* title, int resX, int resY, bool resizable, bool vsync);
		~Window() = default;

		GLFWwindow* getHandle();
		void* getNativeHandle();

		bool getShouldClose();
		void getResolution(int* x, int* y);

		void pollEvents();

	private:
		GLFWwindow* window;
	};
}