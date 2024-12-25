#pragma once

#include "public/defines.h"

struct GLFWwindow;

namespace ryn {
	class Window {
	public:
		using OnResize = void(*)(int width, int height);

		RYN_API Window(const char* title, int resX = 1600, int resY = 900, bool resizable = true, bool vsync = true);
		RYN_API ~Window();

		RYN_API void setResizeCallback(OnResize callback);

		RYN_API void* getNativeHandle();
		RYN_API void* getGlfwHandle();

		RYN_API bool getShouldClose();
		RYN_API void getResolution(int* width, int* height);
		RYN_API void pollEvents();

	private:
		static int windowCount;
		GLFWwindow* window;
		OnResize onResizeCallback;

		static void inputKey(GLFWwindow* win, int key, int scancode, int action, int mods);
		static void inputMouseButton(GLFWwindow* win, int button, int action, int mods);
		static void inputMouseScroll(GLFWwindow* win, double xoffset, double yoffset);
		static void inputMousePosition(GLFWwindow* win);
	};
}
