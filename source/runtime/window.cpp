#include "ryn/core/window.h"
#include "ryn/core/input.h"
#include <iostream>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

ryn::Window::Window(WindowSettings settings) {
	// Set glfw error callback
	glfwSetErrorCallback([](int error, const char* description) {
		std::fprintf(stderr, "GLFW error %d: %s\n", error, description);
		});

	// Initialize glfw if not already
	if (!glfwInit())
		throw std::runtime_error("Failed to initialize GLFW");

	// Create the window
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, settings.resizable ? GLFW_TRUE : GLFW_FALSE);
	window = glfwCreateWindow(settings.width, settings.height, settings.title, nullptr, nullptr);

	// Check if window creation was successful
	if (!window)
		throw std::runtime_error("Failed to create GLFW window");

	// Associate the GLFW window with this ryn::Window instance
	glfwSetWindowUserPointer(window, this);

	// Center window
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(window, (mode->width - settings.width) / 2, (mode->height - settings.height) / 2);

	// Force window to show responding
	glfwShowWindow(window);
	glfwSetCursor(window, nullptr);
	glfwPollEvents();
}

ryn::Window::~Window() {
	glfwDestroyWindow(window);
}

void ryn::Window::pollEvents() {
	ryn::Input::reset();
	glfwPollEvents();
}

bool ryn::Window::isClosing() {
	return glfwWindowShouldClose(window);
}

GLFWwindow* ryn::Window::getGLFWwindowPtr() {
	return window;
}

void* ryn::Window::getNativeWindowPtr() {
#ifdef GLFW_EXPOSE_NATIVE_WIN32
	return glfwGetWin32Window(window);
#elif GLFW_EXPOSE_NATIVE_COCOA
	return glfwGetCocoaWindow(window);
#elif GLFW_EXPOSE_NATIVE_X11
	return glfwGetX11Window(window);
#else
#error "Couldn't find platform specific implementation for Window::getNativeHandle(). Please define the appropriate GLFW_EXPOSE_NATIVE_[...] macro for your platform."
#endif
}