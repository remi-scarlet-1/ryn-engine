#pragma once

#include "public/defines.h"
#include <vector>

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
		using OnSettingsChange = void(*)(const WindowSettings* newSettings);

		Window(WindowSettings settings);
		~Window();

		const WindowSettings* getWindowSettings();
		bool getShouldClose();
		void addSettingsChangeCallback(OnSettingsChange callback);
		static void pollEvents();

		void* getNativeHandle();
		GLFWwindow* getGlfwHandle();

	private:
		static int windowCount;
		WindowSettings settings;
		GLFWwindow* glfwHandle;
		std::vector<OnSettingsChange> onSettingsChangeCallbacks;


		void notifySettingsChange();
		void onWindowResize(int width, int height);
	};
}
