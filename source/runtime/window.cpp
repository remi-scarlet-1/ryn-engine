#include "window.h"

#include "input.h"
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
	glfwHandle = glfwCreateWindow(settings.width, settings.height, settings.title, nullptr, nullptr);

	// Check if window creation was successful
	if (!glfwHandle)
		throw std::runtime_error("Failed to create GLFW window");

	// Associate the GLFW window with this ryn::Window instance
	glfwSetWindowUserPointer(glfwHandle, this);

	// Center window
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(glfwHandle, (mode->width - settings.width) / 2, (mode->height - settings.height) / 2);
}

ryn::Window::~Window() {
	glfwDestroyWindow(glfwHandle);
}

void ryn::Window::pollEvents() {
	ryn::Input::reset();
	glfwPollEvents();
}

GLFWwindow* ryn::Window::getGlfwHandle() {
	return glfwHandle;
}

void* ryn::Window::getNativeHandle() {
#ifdef GLFW_EXPOSE_NATIVE_WIN32
	return glfwGetWin32Window(glfwHandle);
#elif GLFW_EXPOSE_NATIVE_COCOA
	return glfwGetCocoaWindow(window);
#elif GLFW_EXPOSE_NATIVE_X11
	return glfwGetX11Window(window);
#else
#error "Couldn't find platform specific implementation for Window::getNativeHandle(). Please define the appropriate GLFW_EXPOSE_NATIVE_[...] macro for your platform."
#endif
}

bool ryn::Window::getShouldClose() {
	return glfwWindowShouldClose(glfwHandle);
}