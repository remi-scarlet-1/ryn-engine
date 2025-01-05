#include "window.h"

#include "input.h"

#include <iostream>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

int ryn::Window::windowCount = 0;

ryn::Window::Window(WindowSettings settings) : settings(settings), glfwHandle(nullptr) {
	// Set glfw error callback
	glfwSetErrorCallback([](int error, const char* description) {
		std::fprintf(stderr, "GLFW error %d: %s\n", error, description);
	});

	// Initialize glfw if not already
	if (!glfwInit()) {
		glfwTerminate();
		throw std::runtime_error("Failed to initialize GLFW");
	}

	// Create the window
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, settings.resizable ? GLFW_TRUE : GLFW_FALSE);
	glfwHandle = glfwCreateWindow(settings.width, settings.height, settings.title, nullptr, nullptr);
	
	// Check if window creation was successful
	if (!glfwHandle)
		throw std::runtime_error("Failed to create GLFW window");
	windowCount++;

	// Associate the GLFW window with this ryn::Window instance
	glfwSetWindowUserPointer(glfwHandle, this);

	// Center window
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(glfwHandle, (mode->width - settings.width) / 2, (mode->height - settings.height) / 2);

	// Set glfw callbacks
	glfwSetWindowSizeCallback(glfwHandle, [](GLFWwindow* win, int width, int height) {
		Window* windowInstance = static_cast<Window*>(glfwGetWindowUserPointer(win));
		if (windowInstance)
			windowInstance->onWindowResize(width, height);
	});
}

ryn::Window::~Window() {
	// Destroy the current window
	glfwDestroyWindow(glfwHandle);

	// Decrement the window count and shutdown glfw if there are no windows left
	if (--windowCount == 0)
		glfwTerminate();
}

void ryn::Window::addSettingsChangeCallback(OnSettingsChange callback) {
	// Add callback receiver to the onettingsChangeCallbacks vector
	onSettingsChangeCallbacks.push_back(callback);
}

void ryn::Window::pollEvents() {
	ryn::Input::reset();
	glfwPollEvents();
}

bool ryn::Window::getShouldClose() {
	return glfwWindowShouldClose(glfwHandle);
}

const ryn::WindowSettings* ryn::Window::getWindowSettings() {
	return &settings;
}

void* ryn::Window::getNativeHandle() {
#ifdef GLFW_EXPOSE_NATIVE_WIN32
	return glfwGetWin32Window(glfwHandle);
#elif GLFW_EXPOSE_NATIVE_COCOA
	return glfwGetCocoaWindow(window);
#elif GLFW_EXPOSE_NATIVE_X11
	return glfwGetX11Window(window);
#else
#error "You must define the GLFW_EXPOSE_NATIVE_* for your platform"
#endif
}

GLFWwindow* ryn::Window::getGlfwHandle() {
	return glfwHandle; 
}

void ryn::Window::notifySettingsChange() {
	for (const auto& callback : onSettingsChangeCallbacks)
		if (callback)
			callback(&settings);
}

void ryn::Window::onWindowResize(int width, int height) {
	settings.width = width;
	settings.height = height;
	notifySettingsChange();
}
