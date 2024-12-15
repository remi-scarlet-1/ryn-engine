#include "window.h"

#include "input.h"

#include <iostream>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

int ryn::Window::windowCount = 0;

ryn::Window::Window(const char* title, int resX, int resY, bool resizable, bool vsync) : window(nullptr) {
	glfwSetErrorCallback([](int error, const char* description) {
		std::fprintf(stderr, "GLFW error %d: %s\n", error, description);
	});

	if (!glfwInit()) {
		glfwTerminate();
		throw std::runtime_error("Failed to initialize GLFW");
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);

	// Create the window
	window = glfwCreateWindow(resX, resY, title, nullptr, nullptr);
	if (!window) {
		throw std::runtime_error("Failed to create GLFW window");
	}

	windowCount++;

	// Associate the GLFW window with this instance
	glfwSetWindowUserPointer(window, this);

	// Center window
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(window, (mode->width - resX) / 2, (mode->height - resY) / 2);

	// Set callbacks
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
		Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
		if (win && win->onResizeCallback)
			win->onResizeCallback(width, height);
	});

	// Set input callbacks
	glfwSetKeyCallback(window, inputKey);
	glfwSetMouseButtonCallback(window, inputMouseButton);
	glfwSetScrollCallback(window, inputMouseScroll);
}

ryn::Window::~Window() {
	glfwDestroyWindow(window);
	if (--windowCount == 0)
		glfwTerminate();
}

void ryn::Window::setResizeCallback(OnResize callback) {
	onResizeCallback = callback;
}

void* ryn::Window::getNativeHandle() {
#ifdef GLFW_EXPOSE_NATIVE_WIN32
	return glfwGetWin32Window(window);
#elif GLFW_EXPOSE_NATIVE_COCOA
	return glfwGetCocoaWindow(window);
#elif GLFW_EXPOSE_NATIVE_X11
	return glfwGetX11Window(window);
#else
#error "You must define the GLFW_EXPOSE_NATIVE_* for your platform"
#endif
}

GLFWwindow* ryn::Window::getGlfwHandle() { 
	return window; 
}

bool ryn::Window::getShouldClose() {
	return glfwWindowShouldClose(window);
}

void ryn::Window::getResolution(int* width, int* height) {
	glfwGetWindowSize(window, width, height);
}

void ryn::Window::pollEvents() {
	glfwPollEvents(); // Invokes all set Key/Mouse/etc. callbacks
	inputMousePosition(window); // Manual fetching of mouse position
	
	ryn::Input::update();
}

void ryn::Window::inputKey(GLFWwindow* win, int key, int scancode, int action, int mods) {
	// TODO: Implement with input system

}

void ryn::Window::inputMouseButton(GLFWwindow* win, int button, int action, int mods) {
	// TODO: Implement with input system

}

void ryn::Window::inputMouseScroll(GLFWwindow* win, double xoffset, double yoffset) {
	// TODO: Implement with input system

}

void ryn::Window::inputMousePosition(GLFWwindow* win) {
	// TODO: Implement with input system

}