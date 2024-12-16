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
	// Map GLFW key codes to your Key enum
	if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z)
		ryn::Input::updateKeyState(static_cast<ryn::Input::Key>(key - GLFW_KEY_A), action != GLFW_RELEASE);
	else if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9)
		ryn::Input::updateKeyState(static_cast<ryn::Input::Key>(key), action != GLFW_RELEASE);
	else if (key >= GLFW_KEY_F1 && key <= GLFW_KEY_F12)
		ryn::Input::updateKeyState(static_cast<ryn::Input::Key>(key - GLFW_KEY_F1 + static_cast<int>(ryn::Input::Key::F1)), action != GLFW_RELEASE);
	else {
		switch (key) {
		case GLFW_KEY_LEFT: ryn::Input::updateKeyState(ryn::Input::Key::LeftArrow, action != GLFW_RELEASE); break;
		case GLFW_KEY_RIGHT: ryn::Input::updateKeyState(ryn::Input::Key::RightArrow, action != GLFW_RELEASE); break;
		case GLFW_KEY_UP: ryn::Input::updateKeyState(ryn::Input::Key::UpArrow, action != GLFW_RELEASE); break;
		case GLFW_KEY_DOWN: ryn::Input::updateKeyState(ryn::Input::Key::DownArrow, action != GLFW_RELEASE); break;

		case GLFW_KEY_ENTER: ryn::Input::updateKeyState(ryn::Input::Key::Enter, action != GLFW_RELEASE); break;
		case GLFW_KEY_SPACE: ryn::Input::updateKeyState(ryn::Input::Key::Space, action != GLFW_RELEASE); break;
		case GLFW_KEY_TAB: ryn::Input::updateKeyState(ryn::Input::Key::Tab, action != GLFW_RELEASE); break;
		case GLFW_KEY_ESCAPE: ryn::Input::updateKeyState(ryn::Input::Key::Escape, action != GLFW_RELEASE); break;
		case GLFW_KEY_BACKSPACE: ryn::Input::updateKeyState(ryn::Input::Key::Backspace, action != GLFW_RELEASE); break;
		case GLFW_KEY_LEFT_SHIFT: ryn::Input::updateKeyState(ryn::Input::Key::Shift, action != GLFW_RELEASE); break;
		case GLFW_KEY_RIGHT_SHIFT: ryn::Input::updateKeyState(ryn::Input::Key::Shift, action != GLFW_RELEASE); break;
		case GLFW_KEY_LEFT_CONTROL: ryn::Input::updateKeyState(ryn::Input::Key::Control, action != GLFW_RELEASE); break;
		case GLFW_KEY_RIGHT_CONTROL: ryn::Input::updateKeyState(ryn::Input::Key::Control, action != GLFW_RELEASE); break;
		case GLFW_KEY_LEFT_ALT: ryn::Input::updateKeyState(ryn::Input::Key::Alt, action != GLFW_RELEASE); break;
		case GLFW_KEY_RIGHT_ALT: ryn::Input::updateKeyState(ryn::Input::Key::Alt, action != GLFW_RELEASE); break;
		case GLFW_KEY_CAPS_LOCK: ryn::Input::updateKeyState(ryn::Input::Key::CapsLock, action != GLFW_RELEASE); break;
		case GLFW_KEY_NUM_LOCK: ryn::Input::updateKeyState(ryn::Input::Key::NumLock, action != GLFW_RELEASE); break;
		case GLFW_KEY_SCROLL_LOCK: ryn::Input::updateKeyState(ryn::Input::Key::ScrollLock, action != GLFW_RELEASE); break;
		case GLFW_KEY_INSERT: ryn::Input::updateKeyState(ryn::Input::Key::Insert, action != GLFW_RELEASE); break;
		case GLFW_KEY_DELETE: ryn::Input::updateKeyState(ryn::Input::Key::Delete, action != GLFW_RELEASE); break;
		
		case GLFW_KEY_HOME: ryn::Input::updateKeyState(ryn::Input::Key::Home, action != GLFW_RELEASE); break;
		case GLFW_KEY_END: ryn::Input::updateKeyState(ryn::Input::Key::End, action != GLFW_RELEASE); break;
		case GLFW_KEY_PAGE_UP: ryn::Input::updateKeyState(ryn::Input::Key::PageUp, action != GLFW_RELEASE); break;
		case GLFW_KEY_PAGE_DOWN: ryn::Input::updateKeyState(ryn::Input::Key::PageDown, action != GLFW_RELEASE); break;
		case GLFW_KEY_PAUSE: ryn::Input::updateKeyState(ryn::Input::Key::Pause, action != GLFW_RELEASE); break;
		case GLFW_KEY_PRINT_SCREEN: ryn::Input::updateKeyState(ryn::Input::Key::PrintScreen, action != GLFW_RELEASE); break;
		
		default: break;
		}
	}
}

void ryn::Window::inputMouseButton(GLFWwindow* win, int button, int action, int mods) {
	ryn::Input::MouseButton mappedButton;
	switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT: mappedButton = ryn::Input::MouseButton::Left; break;
	case GLFW_MOUSE_BUTTON_RIGHT: mappedButton = ryn::Input::MouseButton::Right; break;
	case GLFW_MOUSE_BUTTON_MIDDLE: mappedButton = ryn::Input::MouseButton::Middle; break;
	default: return;
	}
	ryn::Input::updateMouseButtonState(mappedButton, action != GLFW_RELEASE);
}

void ryn::Window::inputMouseScroll(GLFWwindow* win, double xOffset, double yOffset) {
	ryn::Input::updateMouseScroll(xOffset, yOffset);
}

void ryn::Window::inputMousePosition(GLFWwindow* win) {
	double xPos, yPos;
	glfwGetCursorPos(win, &xPos, &yPos);
	ryn::Input::updateMousePosition(xPos, yPos);
}