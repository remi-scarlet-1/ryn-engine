#pragma once

#include <functional>

// Forward declaration GLFW library.
struct GLFWwindow;

// Engine namespace
namespace ryn {
	// Manages a graphical window with GLFW integration.
	class Window {
	public:
		// Type alias for resize callback functions: accepts new width and height as parameters.
		using OnResize = std::function<void(int, int)>;

		// Constructor: Creates a window with the specified title and resolution.
		// Parameters:
		// - title: Title of the window.
		// - resX, resY: Initial resolution (default: 1600x900).
		// - resizable: Whether the window can be resized (default: true).
		// - vsync: Whether vertical synchronization is enabled (default: true).
		Window(const char* title, int resX = 1600, int resY = 900, bool resizable = true, bool vsync = true);
		~Window();

		// Sets a callback function to be triggered when the window is resized.
		void setResizeCallback(OnResize callback);

		// Returns a native OS-specific handle for the window (e.g., HWND on Windows).
		void* getNativeHandle();
		GLFWwindow* getGlfwHandle();

		bool getShouldClose();
		void getResolution(int* width, int* height);
		void pollEvents();

	private:
		static int windowCount; // Tracks the number of active windows.
		GLFWwindow* window; // Pointer to the GLFW window.
		OnResize onResizeCallback; // User-defined callback for window resizing.

		// Callbacks for glfw inputs
		static void inputKey(GLFWwindow* win, int key, int scancode, int action, int mods);
		static void inputMouseButton(GLFWwindow* win, int button, int action, int mods);
		static void inputMouseScroll(GLFWwindow* win, double xoffset, double yoffset);
		static void inputMousePosition(GLFWwindow* win);
	};
}
