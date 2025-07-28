/**
 * @file		window.h
 * @brief		Provides windowing structures like `ryn::Window` and `ryn::WindowSettings` which wrap GLFW windows. 
 *
 * @author		Konstantinos Lampros
 * @version		1.0.1
 * @date		2025-07-22
 */

#pragma once

#include "ryn/defines.h"
#include "ryn/core/memory.h"

struct GLFWwindow;

namespace ryn {
	/**
	 * @struct	WindowSettings
	 * @brief	Defines a set of settings required for window creation.
	 */
	struct RYN_API WindowSettings {
		const char* title;
		const bool resizable;
		int width, height;
		bool vsync;
	};
	
	/**
	 * @class	Window
	 * @brief	Represents a desktop window and its associated windowing operations.
	 */
	class RYN_API Window {
	public:
		/**
		 * @brief	Constructs and initializes a new instance of `ryn::Window` with the specified settings
		 * @param	settings: A instance of struct `ryn::WindowSettings`.
		 */
		Window(WindowSettings settings);
		~Window() = default;

		/**
		 * @brief	Polls events for all windows.
		 *			This function should be called regularly to process input events.
		 */
		static void pollEvents();

		/**
		 * @brief	Checks whether the window has been requested to close.
		 * @return	true if the window should close; false otherwise.
		 */
		bool isClosing();

		/**
		 * @brief	Retrieves the underlying GLFW window instance.
		 *			Can be used for low-level operations or integration with external libraries.
		 * @return	A pointer to the `GLFWwindow` instance.
		 */
		GLFWwindow* getGLFWwindowPtr();

		/**
		 * @brief	Retrieves the native OS handle for the underlying window.
		 *			May vary depending on operating and windowing system. Can be used for integration
		 *			with external libraries.
		 * @return	A pointer to the windows native OS handle.
		 */
		void* getNativeWindowPtr();

	private:
		UniquePtr<GLFWwindow, void(*)(GLFWwindow*)> window;
	};
}