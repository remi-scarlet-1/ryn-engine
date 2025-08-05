#pragma once

#include "ryn/defines.h"
#include "ryn/subsystem.h"

struct GLFWwindow;

namespace ryn::core {
	enum RYN_API GraphicsAPI {
		None = 0,
		OpenGL,
	};

	enum RYN_API VsyncMode {
		Off = 0,
		EveryVBlank,
		EverySecondVBlank,
	};

	const struct RYN_API WindowSettings {
		const char* title;
		const bool resizable;
		const int width, height;
		const GraphicsAPI graphicsAPI;
		const VsyncMode vsyncMode;
	};

	class RYN_API Window : public Subsystem {
	public:
		class Backend {
		public:
			static void initialize();
			static void pollEvents();
		};

		class OpenGLContext {
		public:
			static void clear();
			static void activate(Window& window);
			static void swapFramebuffers(Window& window);
		};

	public:
		Window(const WindowSettings& settings);
		~Window() = default;
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		bool isClosing();
		void* getNativeWindowPtr();
		GLFWwindow* getGLFWwindowPtr();

	protected:
		void initialize() override;
		void update() override;
		void shutdown() override;

	private:
		GLFWwindow* m_window;
		WindowSettings m_settings;
	};
}