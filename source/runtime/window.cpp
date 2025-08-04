#include "ryn/graphics/window.h"

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <iostream>

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

void ryn::Window::OpenGLContext::clear() {
	glfwMakeContextCurrent(nullptr);
}

void ryn::Window::OpenGLContext::activate(Window& window) {
	if (window.m_settings.graphicsAPI == GraphicsAPI::OpenGL) {
		auto [ok, ptr] = window.getGLFWwindowPtr().tryGet();
		glfwMakeContextCurrent(ptr);
	}
}

void ryn::Window::OpenGLContext::swapFramebuffers(Window& window) {
	if (window.m_settings.graphicsAPI == GraphicsAPI::OpenGL)
		if (auto [ok, ptr] = window.getGLFWwindowPtr().tryGet(); ok)
			glfwSwapBuffers(ptr);
}

ryn::Window::Window(const WindowSettings& settings) : m_settings(settings) {
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

	m_ptr = UniquePtr<GLFWwindow, void(*)(GLFWwindow*)>(
		glfwCreateWindow(m_settings.width, m_settings.height, m_settings.title, nullptr, nullptr),
		glfwDestroyWindow
	);

	if (!m_ptr)
		throw std::runtime_error("Failed to create GLFW window.");

	glfwMakeContextCurrent(m_ptr.get());

	// Associate the GLFWwindow instance with this instance of ryn::Window
	glfwSetWindowUserPointer(m_ptr.get(), this);

	const GLFWvidmode* videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(m_ptr.get(), (videoMode->width - m_settings.width) / 2, (videoMode->height - m_settings.height) / 2);
	glfwSetCursor(m_ptr.get(), nullptr);
	glfwShowWindow(m_ptr.get());
	glfwPollEvents();
}

bool ryn::Window::isClosing() {
	return glfwWindowShouldClose(m_ptr.get());
}

void* ryn::Window::getNativeWindowPtr() {
#ifdef GLFW_EXPOSE_NATIVE_WIN32
	return glfwGetWin32Window(m_ptr.get());
#elif GLFW_EXPOSE_NATIVE_COCOA
	return glfwGetCocoaWindow(m_ptr.get());
#elif GLFW_EXPOSE_NATIVE_X11
	return glfwGetX11Window(m_ptr.get());
#else
#error "Couldn't find platform specific implementation for Window::getNativeHandle(). Please define the appropriate GLFW_EXPOSE_NATIVE_[...] macro for your platform."
#endif
}

ryn::OptionalPtr<GLFWwindow> ryn::Window::getGLFWwindowPtr() {
	return OptionalPtr<GLFWwindow>(m_ptr.get());
}