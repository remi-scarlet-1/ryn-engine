/**
 * @file		window.h
 * @brief		Defines the Window subsystem, backend window event polling, and OpenGL context handling.
 * @warning		Not thread-safe. Intended for internal engine use only.
 *
 * @author		Konstantinos Lampros
 * @version		1.0.0
 * @date		2025-08-05
 */

#pragma once

#include "ryn/defines.h"
#include "ryn/subsystem.h"

struct GLFWwindow;

namespace ryn::core {
	/**
	 * @enum	GraphicsAPI
	 * @brief	Represents supported graphics APIs for rendering context creation.
	 */
	enum RYN_API GraphicsAPI {
		None = 0,
		OpenGL,
	};

	/**
	 * @enum	VsyncMode
	 * @brief	Represents available vertical synchronization (VSync) modes for frame presentation.
	 */
	enum RYN_API VsyncMode {
		Off = 0,
		EveryVBlank,
		EverySecondVBlank,
	};

	/**
	 * @struct	WindowSettings
	 * @brief	Describes configuration for creating and initializing a Window.
	 */
	const struct RYN_API WindowSettings {
		/** @brief Title of the window (string not owned). */
		const char* title;
		/** @brief Whether the window can be resized by the user. */
		bool resizable;
		/** @brief Initial window width in pixels. */
		int width;
		/** @brief Initial window height in pixels. */
		int height;
		/** @brief Rendering backend to use for this window. */
		GraphicsAPI graphicsAPI;
		/** @brief VSync mode to control frame presentation timing. */
		VsyncMode vsyncMode;
	};

	/**
	 * @class	Window
	 * @brief	Subsystem managing the native OS window, input event polling, and rendering context activation.
	 * @details	Created with immutable settings via WindowSettings. Provides access to the native window pointer and handles lifecycle methods.
	 */
	class RYN_API Window : public Subsystem {
	public:
		/**
		 * @class	Backend
		 * @brief	Provides static utility functions for global window system setup and event polling.
		 */
		class Backend {
		public:
			/** @brief Initializes backend-specific resources. */
			static void initialize();
			/** @brief Polls input and window events. */
			static void pollEvents();
		};

		/**
		 * @class	OpenGLContext
		 * @brief	Provides OpenGL-specific context management utilities.
		 */
		class OpenGLContext {
		public:
			/**
			 * @brief	Clears the framebuffer using current OpenGL state.
			 */
			static void clear();

			/**
			 * @brief	Makes the OpenGL context of the given window current on the calling thread.
			 * @param	window Reference to Window instance.
			 */
			static void activate(Window& window);

			/**
			 * @brief	Swaps the front and back framebuffers of the given window.
			 * @param	window Reference to Window instance.
			 */
			static void swapFramebuffers(Window& window);
		};

	public:
		Window(const WindowSettings& settings);
		~Window() = default;
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		/**
		 * @brief	Checks whether the window is in the process of closing.
		 * @return	true if the window should close, false otherwise.
		 */
		bool isClosing();

		/**
		 * @brief	Retrieves the raw native window pointer (platform-dependent).
		 * @return	void pointer to the native window (e.g., HWND, X11, Cocoa).
		 */
		void* getNativeWindowPtr();

		/**
		 * @brief	Retrieves the underlying GLFW window pointer.
		 * @return	Pointer to GLFWwindow.
		 */
		GLFWwindow* getGLFWwindowPtr();

	protected:
		void onInitialize() override;
		void onUpdate() override;
		void onShutdown() override;

	private:
		GLFWwindow* m_window;
		WindowSettings m_settings;
	};
}