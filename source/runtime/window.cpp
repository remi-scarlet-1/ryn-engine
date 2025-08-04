#include "ryn/core/window.h"
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <iostream>

#pragma region Backend
void ryn::Window::Backend::initialize() {
	glfwSetErrorCallback([](int error, const char* description) {
		std::fprintf(stderr, "GLFW error %d: %s\n", error, description);
	});

	if (!glfwInit())
		throw std::runtime_error("Failed to initialize windowing backend GLFW");
}

void ryn::Window::Backend::pollEvents() {
	glfwPollEvents();
}
#pragma endregion

#pragma region OpenGLContext
void ryn::Window::OpenGLContext::clear() {
	glfwMakeContextCurrent(nullptr);
}

void ryn::Window::OpenGLContext::activate(Window& window) {
	if (window.m_settings.graphicsAPI == GraphicsAPI::OpenGL)
		glfwMakeContextCurrent(window.getGLFWwindowPtr());
}

void ryn::Window::OpenGLContext::swapFramebuffers(Window& window) {
	if (window.m_settings.graphicsAPI == GraphicsAPI::OpenGL)
		glfwSwapBuffers(window.getGLFWwindowPtr());
}
#pragma endregion

ryn::Window::Window(const WindowSettings& settings) : m_window(nullptr), m_settings(settings) {}

bool ryn::Window::isClosing() {
	return glfwWindowShouldClose(m_window);
}

void* ryn::Window::getNativeWindowPtr() {
#ifdef GLFW_EXPOSE_NATIVE_WIN32
	return glfwGetWin32Window(m_window);
#elif GLFW_EXPOSE_NATIVE_COCOA
	return glfwGetCocoaWindow(m_window);
#elif GLFW_EXPOSE_NATIVE_X11
	return glfwGetX11Window(m_window);
#else
#error "Couldn't find platform specific implementation for Window::getNativeHandle(). Please define the appropriate GLFW_EXPOSE_NATIVE_[...] macro for your platform."
#endif
}

GLFWwindow* ryn::Window::getGLFWwindowPtr() {
	return m_window;
}

void ryn::Window::initialize() {
	Window::Backend::initialize();

	switch (m_settings.graphicsAPI) {
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
	glfwWindowHint(GLFW_RESIZABLE, m_settings.resizable ? GLFW_TRUE : GLFW_FALSE);
	m_window = glfwCreateWindow(m_settings.width, m_settings.height, m_settings.title, nullptr, nullptr);

	if (!m_window)
		throw std::runtime_error("Failed to create GLFW window.");

	glfwSetWindowUserPointer(m_window, this); // Associate the GLFWwindow instance with this instance of ryn::Window

	const GLFWvidmode* videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(m_window, (videoMode->width - m_settings.width) / 2, (videoMode->height - m_settings.height) / 2);
	glfwSetCursor(m_window, nullptr);
	glfwShowWindow(m_window);

	Window::Backend::pollEvents();
}

void ryn::Window::update() {
	Window::Backend::pollEvents();
	if (isClosing())
		getGame().terminate();
}

void ryn::Window::shutdown() {
	glfwDestroyWindow(m_window);
}
