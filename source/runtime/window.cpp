#include "ryn/graphics/window.h"

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <glad/glad.h>

#include <iostream>

void ryn::Window::GLFWBackend::initialize() {
	glfwSetErrorCallback([](int error, const char* description) {
		std::fprintf(stderr, "GLFW error %d: %s\n", error, description);
	});

	if (!glfwInit())
		throw std::runtime_error("Failed to initialize GLFW");
}

void ryn::Window::GLFWBackend::pollEvents() {
	glfwPollEvents();
}

void ryn::Window::GLFWBackend::makeGLContextCurrent(Window& window) {
	glfwMakeContextCurrent(window.getGLFWwindowPtr());
}

void ryn::Window::GLFWBackend::clearGLContext() {
	glfwMakeContextCurrent(nullptr);
}

ryn::Window::Window(const WindowSettings& settings) {
	switch (settings.graphicsAPI) {
	case None:
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		break;

	case OpenGL:
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		break;
	}

	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, settings.resizable ? GLFW_TRUE : GLFW_FALSE);

	windowPtr = UniquePtr<GLFWwindow, void(*)(GLFWwindow*)>(
		glfwCreateWindow(settings.width, settings.height, settings.title, nullptr, nullptr),
		glfwDestroyWindow
	);

	if (!windowPtr)
		throw std::runtime_error("Failed to create GLFW window.");

	glfwMakeContextCurrent(windowPtr.get());

	// Associate the GLFWwindow instance with this instance of ryn::Window
	glfwSetWindowUserPointer(windowPtr.get(), this);

	const GLFWvidmode* videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(windowPtr.get(), (videoMode->width - settings.width) / 2, (videoMode->height - settings.height) / 2);
	glfwSetCursor(windowPtr.get(), nullptr);
	glfwShowWindow(windowPtr.get());
	glfwPollEvents();
}

bool ryn::Window::isClosing() {
	return glfwWindowShouldClose(windowPtr.get());
}

void* ryn::Window::getNativeWindowPtr() {
#ifdef GLFW_EXPOSE_NATIVE_WIN32
	return glfwGetWin32Window(windowPtr.get());
#elif GLFW_EXPOSE_NATIVE_COCOA
	return glfwGetCocoaWindow(window.get());
#elif GLFW_EXPOSE_NATIVE_X11
	return glfwGetX11Window(window.get());
#else
#error "Couldn't find platform specific implementation for Window::getNativeHandle(). Please define the appropriate GLFW_EXPOSE_NATIVE_[...] macro for your platform."
#endif
}

GLFWwindow* ryn::Window::getGLFWwindowPtr() {
	return windowPtr.get();
}