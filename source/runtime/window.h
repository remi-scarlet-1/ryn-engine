/**
* @file    window.h
* @brief   Provides a 'Window' class and 'WindowSettings' structure for managing the windowing system.
*          This file provides basic functionality to create and interact with a GLFW window, handle events and query window properties.
* @author  Konstantinos Lampros
* @version 1.0.0
* @date    2025-01-12
*/

#pragma once

#include "public/defines.h"

// Forward declaration of GLFWwindow type
struct GLFWwindow;

namespace ryn {

	/**
	* @struct WindowSettings
	* @brief Defines the settings required for creating a window.
	*/
	struct WindowSettings {
		const char* title;		// The title of the window.
		const bool resizable;	// Whether the window is resizable
		int width, height;		// The width and height of the window in pixels.
		bool vsync;				// Whether vertical synchronization (V-Sync) is enabled. 
	};
	
	/**
	* @class Window
	* @brief Represents a window and its associated windowing operations.
	*
	* The 'Window' class provides basic functionality to create a window using GLFW, manage window events, and query the window's state.
	*/
	class Window {
	public:
		/**
		* @brief Constructs a new Window object with the specified settings.
		*
		* This constructor creates a new window using the provided 'WindowSettings' configuration.
		* It initializes the window and prepares it for use.
		*
		* @param settings The settings for the window (e.g., title, size, resizable, vsync, etc.).
		*/
		Window(WindowSettings settings);
		~Window();

		// Hack: disable copying of windows to prevent accidental calls to the destructor.
		// By deleting these functions, we ensure the window cannot be copied, which would lead to double destruction.
		Window(const Window&) = delete;            // Disable copy constructor
		Window& operator=(const Window&) = delete; // Disable copy assignment

		/**
		* @brief Polls events for all windows.
		*
		* This function should be called regularly (typically every frame) to process input events,
		* such as keyboard, mouse, and window events.
		*/
		static void pollEvents();

		/**
		* @brief Retrieves the GLFW handle for the window.
		*
		* This function returns the native GLFW window handle, which can be used for low-level
		* operations or integrating with other libraries that require direct access to the GLFW window.
		*
		* @return A pointer to the GLFW window handle.
		*/
		GLFWwindow* getGlfwHandle();

		/**
		* @brief Retrieves the native handle for the window.
		*
		* This function provides access to the native window handle, which may vary depending on the
		* operating system and the windowing system. It is used for external libraries that need access
		* to the actual window handle.
		*
		* @return A pointer to the native window handle.
		*/
		void* getNativeHandle();

		/**
		* @brief Determines whether the window should close
		* @return 'true' if the window should close, 'false' otherwise.
		*/
		bool getShouldClose();

	private:
		// The handle to the GLFW window instance.
		GLFWwindow* glfwHandle;
	};
}