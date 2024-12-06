#include "Window.h"

#include <iostream>
#include <GLFW/glfw3native.h>

static void glfw_errorCallback(int error, const char* description) {
	std::fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

static void glfw_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	// TODO: implement keyboard input
}

rn::Window::Window(const char* title, int resX, int resY, bool resizable, bool vsync) : window(nullptr)
{
	glfwSetErrorCallback(glfw_errorCallback);
	if (!glfwInit()) {
		throw std::runtime_error("Failed to initialize GLFW");
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);

	window = glfwCreateWindow(resX, resY, title, nullptr, nullptr);

	if (!window) {
		throw std::runtime_error("Failed to create GLFW window");
	}

	// Subscribe to key inputs
	glfwSetKeyCallback(window, glfw_keyCallback);

	// Center window
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(window, (mode->width - resX) / 2, (mode->height - resY) / 2);
}

GLFWwindow* rn::Window::getHandle() { 
	return window; 
}

void* rn::Window::getNativeHandle() {
#ifdef GLFW_EXPOSE_NATIVE_WIN32
	return glfwGetWin32Window(window);
#else
	#error "You must define GLFW_EXPOSE_NATIVE_*"
#endif // GLFW_EXPOSE_NATIVE_WIN32
}

bool rn::Window::getShouldClose() {
	return glfwWindowShouldClose(window);
}

void rn::Window::getResolution(int* x, int* y) {
	glfwGetWindowSize(window, x, y);
}

void rn::Window::pollEvents() {
	glfwPollEvents();
}