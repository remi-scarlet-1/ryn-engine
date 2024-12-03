#pragma once

//#include <chrono>
//#include <functional>
//#include <GLFW/glfw3.h>
//#include <glm/vec2.hpp>
//
//namespace rn {
//	class Window {
//	public:
//		Window(const char* title, glm::ivec2 resolution, bool resizable, bool vsync, std::function<void(float)> renderCallback);
//		~Window() = default;
//
//		void presentFrame();
//		void pollEvents();
//		void close();
//
//		const GLFWwindow* getWindowHandle();
//		const float getDeltaTime();
//
//		const bool shouldClose();
//
//		void* (*getGLFunctionLoader())(const char*);
//
//	private:
//		// Tracks open window count to ensure windows <= 1 (temporary restriction, subject to change)
//		// TODO: implement support for multiple windows, make threadsafe
//		static int totalWindowCount;
//		
//		GLFWwindow* window;
//		std::function<void(float)> renderCallback;
//
//		float deltaTime;
//		std::chrono::high_resolution_clock::time_point lastPresentTime;
//	};
//}