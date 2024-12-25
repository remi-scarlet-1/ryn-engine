#pragma once

#include "public/defines.h"
#include <vector>

struct GLFWwindow;

namespace ryn {
	struct RYN_API WindowSettings {
		const char* title;
		int width, height;
		const bool resizable;
		bool vsync;
	};

	class Window {
	public:
		using OnSettingsChange = void(*)(const WindowSettings* newSettings);

		RYN_API Window(WindowSettings settings);
		RYN_API ~Window();

		RYN_API const WindowSettings* getWindowSettings();
		RYN_API bool getShouldClose();
		RYN_API void* getNativeHandle();
		RYN_API void* getGlfwHandle();
		RYN_API void addSettingsChangeCallback(OnSettingsChange callback);

		RYN_API static void pollEvents();

	private:
		static int windowCount;
		WindowSettings settings;
		GLFWwindow* glfwHandle;
		std::vector<OnSettingsChange> onSettingsChangeCallbacks;

		void notifySettingsChange();
		void onWindowResize(int width, int height);
	};
}
