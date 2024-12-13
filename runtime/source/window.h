#pragma once

#include <functional>

// Forward declaration GLFW library.
struct GLFWwindow;

namespace rn {
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

		// Destructor: Cleans up the window and associated resources.
		~Window();

		// Sets a callback function to be triggered when the window is resized.
		void setResizeCallback(OnResize callback);

		// Returns a native OS-specific handle for the window (e.g., HWND on Windows).
		void* getNativeHandle();

		// Returns the underlying GLFW handle for the window.
		GLFWwindow* getGlfwHandle();

		// Checks if the window should close (e.g., user clicked the close button).
		bool getShouldClose();

		// Retrieves the current window resolution.
		// Parameters:
		// - x, y: Pointers to store the width and height of the window.
		void getResolution(int* x, int* y);

		// Processes input and window events. Call this once per frame.
		void pollEvents();

	private:
		static int windowCount; // Tracks the number of active windows.

		GLFWwindow* window; // Pointer to the GLFW window.

		OnResize onResizeCallback; // User-defined callback for window resizing.
	};
}
