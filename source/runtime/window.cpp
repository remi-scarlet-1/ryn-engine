#include "ryn/graphics/window.h"
#include "ryn/core/input.h"

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <glad/glad.h>

#include <iostream>

ryn::Window::Window(WindowSettings settings) {
	// Set glfw error callback
	glfwSetErrorCallback([](int error, const char* description) {
		std::fprintf(stderr, "GLFW error %d: %s\n", error, description);
	});

	// Initialize glfw if not already
	if (!glfwInit())
		throw std::runtime_error("Failed to initialize GLFW");

	// Create the window
	//glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, settings.resizable ? GLFW_TRUE : GLFW_FALSE);

	window = UniquePtr<GLFWwindow, void(*)(GLFWwindow*)>(
		glfwCreateWindow(settings.width, settings.height, settings.title, nullptr, nullptr),
		glfwDestroyWindow
	);

	// Check if window creation was successful
	if (!window)
		throw std::runtime_error("Failed to create GLFW window");

	glfwMakeContextCurrent(window.get());
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw std::runtime_error("Failed to initialize GLAD\n");

	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

	// Associate the GLFW window with this ryn::Window instance
	glfwSetWindowUserPointer(window.get(), this);

	// Center window
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(window.get(), (mode->width - settings.width) / 2, (mode->height - settings.height) / 2);

	// Force window to show responding
	glfwShowWindow(window.get());
	glfwSetCursor(window.get(), nullptr);
	glfwPollEvents();
}

void ryn::Window::pollEvents() {
	ryn::Input::reset();
	glfwPollEvents();
}

bool ryn::Window::isClosing() {
	return glfwWindowShouldClose(window.get());
}

GLFWwindow* ryn::Window::getGLFWwindowPtr() {
	return window.get();
}

void* ryn::Window::getNativeWindowPtr() {
#ifdef GLFW_EXPOSE_NATIVE_WIN32
	return glfwGetWin32Window(window.get());
#elif GLFW_EXPOSE_NATIVE_COCOA
	return glfwGetCocoaWindow(window.get());
#elif GLFW_EXPOSE_NATIVE_X11
	return glfwGetX11Window(window.get());
#else
#error "Couldn't find platform specific implementation for Window::getNativeHandle(). Please define the appropriate GLFW_EXPOSE_NATIVE_[...] macro for your platform."
#endif
}