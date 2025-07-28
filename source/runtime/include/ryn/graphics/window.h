/**
 * @file		window.h
 * @brief		Provides platform agnostic windowing API, wraps GLFW. 
 *
 * @author		Konstantinos Lampros
 * @version		1.1.0
 * @date		2025-07-28
 */

#pragma once

#include "ryn/defines.h"
#include "ryn/graphics/enums.h"
#include "ryn/core/memory.h"

struct GLFWwindow;

namespace ryn {
	/**
	 * @struct	WindowSettings
	 * @brief	Defines a set of settings required for window creation.
	 */
	const struct RYN_API WindowSettings {
		const char* title;
		const bool resizable;
		const int width, height;
		const GraphicsAPI graphicsBackend;
		const VsyncMode vsyncMode;
	};
	
	/**
	 * @class	Window
	 * @brief	Represents a desktop window and its associated windowing operations.
	 */
	class RYN_API Window {
	public:
		/**
		 * @class	GLFWBackend
		 *			Acts as namespace for static functionality related to the windowing backend.
		 */
		class GLFWBackend {
		public:
			/**
			 * @brief	Initializes the windowing backend.
			 *			Must be called first in order to create instances of `ryn::Window`.
			 */
			static void initialize();

			/**
			 * @brief	Polls events for all windows.
			 *			Should be called regularly to process input events.
			 */
			static void pollEvents();

			/**
			 * @brief	Activates the window's OpenGL context on the calling thread
			 *			This function is only meaningful if the passed window has been created using the
			 *			OpenGL `GraphicsAPI` option
			 * @param	window: The window of which to activate the OpenGL context
			 * @note	Calling this function with any other `GraphicsAPI` backends then `OpenGL` has no effect.
			 */
			static void makeGLContextCurrent(Window& window);

			/**
			 * @brief	Deactivate any current OpenGL context on the calling thread
			 * @note	Calling this function with any other `GraphicsAPI` backends then `OpenGL` has no effect.
			 */
			static void clearGLContext();
		};

		/**
		 * @brief	Constructs and initializes a new instance of `ryn::Window` with the specified settings
		 * @param	settings: A instance of struct `ryn::WindowSettings`.
		 */
		Window(const WindowSettings& settings);
		~Window() = default;

		/**
		 * @brief	Checks whether the window has been requested to close.
		 * @return	true if the window should close; false otherwise.
		 */
		bool isClosing();

		/**
		 * @brief	Retrieves the native OS handle for the underlying window.
		 *			May vary depending on operating and windowing system. Can be used for integration
		 *			with external libraries.
		 * @return	A pointer to the windows native OS handle.
		 */
		void* getNativeWindowPtr();

		/**
		 * @brief	Retrieves the underlying GLFW window instance.
		 *			Can be used for low-level operations or integration with external libraries.
		 * @return	A pointer to the `GLFWwindow` instance.
		 */
		GLFWwindow* getGLFWwindowPtr();

	private:
		UniquePtr<GLFWwindow, void(*)(GLFWwindow*)> windowPtr;
	};
}