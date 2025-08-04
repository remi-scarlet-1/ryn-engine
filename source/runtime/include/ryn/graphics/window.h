/**
 * @file		window.h
 * @brief		Platform-agnostic windowing API.
 *
 * @author		Konstantinos Lampros
 * @version		1.2.1
 * @date		2025-08-04
 */

#pragma once

#include "ryn/defines.h"
#include "ryn/core/memory.h"

struct GLFWwindow;

namespace ryn {
	/**
	 * @enum	GraphicsAPI
	 * @brief	Available graphics API backends.
	 */
	enum RYN_API GraphicsAPI {
		None = 0,
		OpenGL,
	};

	/**
	 * @enum	VsyncMode
	 * @brief	Framerate limiting modes.
	 */
	enum RYN_API VsyncMode {
		Off = 0,
		EveryVBlank,
		EverySecondVBlank,
	};

	/**
	 * @struct	WindowSettings
	 * @brief	Settings for window creation.
	 */
	const struct RYN_API WindowSettings {
		const char* title;
		const bool resizable;
		const int width, height;
		const GraphicsAPI graphicsAPI;
		const VsyncMode vsyncMode;
	};
	
	/**
	 * @class	Window
	 * @brief	Desktop window and associated operations.
	 */
	class RYN_API Window {
	public:
		/**
		 * @class	Backend
		 * @brief	Windowing backend interface (GLFW).
		 */
		class Backend {
		public:
			/**
			 * @brief	Initializes the windowing backend.
			 * @note	Call before creating any `ryn::Window` instances.
			 */
			static void initialize();

			/**
			 * @brief	Polls input events for all windows.
			 * @note	Must be called regularly to avoid the window or inputs to become sluggish.
			 */
			static void pollEvents();
		};

		/**
		 * @class	OpenGLContext
		 * @brief	Manage OpenGL context on the calling thread.
		 * @warning	All methods operate on the calling thread's active OpenGL context. Ensure correct threading.
		 *			Calling without an active context may fail silently or cause errors.
		 */
		class OpenGLContext {
		public:
			/**
			 * @brief	Clears the calling threads active OpenGL context.
			 */
			static void clear();

			/**
			 * @brief	Activates the specified window's OpenGL context on the calling thread.
			 * @param	window: Reference to the target window. Must use `GraphicsAPI::OpenGL`.
			 */
			static void activate(Window& window);

			/**
			 * @brief	Swaps the framebuffers of the specified window.
			 * @param	window: Reference to the target window.
			 */
			static void swapFramebuffers(Window& window);
		};

		/**
		 * @brief	Creates a window with the specified settings.
		 * @param	settings: The chosen settings for window initialization.
		 */
		Window(const WindowSettings& settings);
		~Window() = default;

		/**
		 * @brief	Checks whether the window has been requested to close.
		 * @return	True if the window should close; False otherwise.
		 */
		bool isClosing();

		/**
		 * @brief	Retrieves pointer to OS native window.
		 *			Can be used for integration with external libraries.
		 * @return	A pointer of unknown type to OS native window.
		 */
		void* getNativeWindowPtr();

		/**
		 * @brief	Retrieves pointer to underlying GLFWwindow.
		 *			Can be used for integration with external libraries.
		 * @return	A pointer to the `GLFWwindow` instance.
		 */
		OptionalPtr<GLFWwindow> getGLFWwindowPtr();

	private:
		UniquePtr<GLFWwindow, void(*)(GLFWwindow*)> m_ptr;
		WindowSettings m_settings;
	};
}