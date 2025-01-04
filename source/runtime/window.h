#pragma once

#include "public/defines.h"
#include <vector>

struct GLFWwindow;

namespace ryn {
	struct RYN_API WindowSettings {
		const char* title;
		const bool resizable;
		int width, height;
		bool vsync;
	};

	class Window {
	public:
		using OnSettingsChange = void(*)(const WindowSettings* newSettings);

		RYN_API static Window* createWindow(WindowSettings windowSettings);
		RYN_API static void destroyWindow(Window* window);

		RYN_API const WindowSettings* getWindowSettings();
		RYN_API bool getShouldClose();
		RYN_API void addSettingsChangeCallback(OnSettingsChange callback);
		RYN_API static void pollEvents();

		void* getNativeHandle();
		GLFWwindow* getGlfwHandle();

	private:
		static int windowCount;
		WindowSettings settings;
		GLFWwindow* glfwHandle;
		std::vector<OnSettingsChange> onSettingsChangeCallbacks;

		explicit Window(WindowSettings settings);
		~Window();

		void notifySettingsChange();
		void onWindowResize(int width, int height);
	};
}
