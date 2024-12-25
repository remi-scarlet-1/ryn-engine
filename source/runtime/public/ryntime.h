#include "defines.h"

namespace ryn {
	struct RYN_API WindowSettings {
		const char* title;
		int width, height;
		const bool resizable;
		bool vsync;
	};

	class Window {
	public:
		using OnSettingsChange = void(*)(const WindowSettings* newSettings);

		RYN_API Window(WindowSettings settings);
		RYN_API ~Window();

		RYN_API const WindowSettings* getWindowSettings();
		RYN_API bool getShouldClose();
		RYN_API void* getNativeHandle();
		RYN_API void* getGlfwHandle();
		RYN_API void addSettingsChangeCallback(OnSettingsChange callback);

		RYN_API static void pollEvents();
	};

	class Input {
	public:
		enum class Key {
			A = 0, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
			Zero, One, Two, Three, Four, Five, Six, Seven, Eight, Nine,
			F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
			KeyCount = 63,

			LeftArrow = 64, RightArrow, UpArrow, DownArrow,
			Enter, Space, Tab, Escape, Backspace, Shift, Control, Alt, CapsLock, NumLock, ScrollLock,
			Insert, Delete, Home, End, PageUp, PageDown, Pause, PrintScreen,
			Minus, Equal, LeftBracket, RightBracket, Backslash, Semicolon, Apostrophe, Comma, Period, Slash,
			ExtendedKeyCount = 127
		};

		enum class MouseButton {
			Left = 0,
			Right,
			Middle,
			Button4,
			Button5,
			Count = 16
		};

		RYN_API static void registerWindow(Window window);

		RYN_API static bool isKeyPressed(Key key);
		RYN_API static bool isKeyReleased(Key key);
		RYN_API static bool isKeyHeld(Key key);

		RYN_API static bool isMousePressed(MouseButton button);
		RYN_API static bool isMouseReleased(MouseButton button);
		RYN_API static bool isMouseHeld(MouseButton button);

		RYN_API static void getMousePosition(double& xPos, double& yPos);
		RYN_API static void getMouseScroll(double& xOffset, double& yOffset);
	};
}