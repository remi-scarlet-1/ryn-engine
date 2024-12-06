#include "Window.h"

#include <iostream>
#include <chrono>

int rn::Window::totalWindowCount = 0;

rn::Window::Window(const char* title, int resX, int resY, bool resizable, bool vsync, std::function<void(float)> renderCallback)
	: window(nullptr), renderCallback(renderCallback), deltaTime(.0f), lastPresentTime(std::chrono::high_resolution_clock::now())
{
	if (totalWindowCount >= 1) {
		throw std::runtime_error("You currently cant open more then 1 window simultaniously. Requires implementation.");
	}

	if (!glfwInit()) {
		throw std::runtime_error("Failed to initialize GLFW");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);

	window = glfwCreateWindow(resX, resY, title, nullptr, nullptr);
	totalWindowCount++;

	if (!window) {
		throw std::runtime_error("Failed to create GLFW window");
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(vsync ? 1 : 0);

	// Center window
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	int windowWidth, windowHeight;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);
	glfwSetWindowPos(window, (mode->width - windowWidth) / 2, (mode->height - windowHeight) / 2);

	// Setup
	lastPresentTime = std::chrono::high_resolution_clock::now();
	deltaTime = 0.0f;
}

void rn::Window::presentFrame()
{
	glfwMakeContextCurrent(window);
	if (renderCallback)
		renderCallback(deltaTime);
	glfwSwapBuffers(window);

	// Calculate delta time
	std::chrono::high_resolution_clock::time_point currentPresentTime = std::chrono::high_resolution_clock::now();
	deltaTime = std::chrono::duration<float>(currentPresentTime - lastPresentTime).count();
	lastPresentTime = currentPresentTime;
}

void rn::Window::pollEvents()
{
	glfwPollEvents();
}

void rn::Window::close()
{
	if (window) {
		glfwDestroyWindow(window);
		window = nullptr;
	}
	if (--totalWindowCount <= 0) {
		glfwTerminate();
	}
}

const GLFWwindow* rn::Window::getWindowHandle() { return window; }
const float rn::Window::getDeltaTime() { return deltaTime; }

const bool rn::Window::shouldClose() { return glfwWindowShouldClose(window) == GLFW_FALSE; }

// Wrap the window libraries function loader implementation
void* glFunctionLoaderWrapper(const char* functionName) { return reinterpret_cast<void*>(glfwGetProcAddress(functionName)); }
void* (*rn::Window::getGLFunctionLoader())(const char*) { return &glFunctionLoaderWrapper; }
