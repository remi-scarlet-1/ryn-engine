#pragma once

#include <functional>

struct GLFWwindow;

namespace rn {
	class Window {
	public:
		using OnResize = std::function<void(int, int)>;
		
		Window(const char* title, int resX = 1600, int resY = 900, bool resizable = true, bool vsync = true);
		~Window();

		void setResizeCallback(OnResize callback);

		void* getNativeHandle();
		GLFWwindow* getGlfwHandle();
		bool getShouldClose();
		void getResolution(int* x, int* y);

		void pollEvents();

	private:
		static int windowCount;
		GLFWwindow* window;
		
		OnResize onResizeCallback;
	};
}