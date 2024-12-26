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

	/**
	* @enum Input::Key
	* @brief Represents all possible keys on the keyboard.
	*
	* Defines all keys that can be monitored by the input system.
	* It includes standard alphanumeric keys (A-Z, 0-9), function keys (F1-F12), arrow keys,
	* and special keys such as Enter, Escape, Shift, etc.
	*
	* @note The enum values are internally mapped to GLFW key codes.
	*/
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

	/**
	* @enum Input::MouseButton
	* @brief Represents all buttons on the mouse.
	*
	* Defines all buttons on the mouse that can be monitored by the input system.
	* It includes typical mouse buttons like left, right, middle (scroll wheel), and additional
	* buttons such as Button4 and Button5 commonly found on gaming mice (side buttons).
	*/
	enum class MouseButton {
		Left = 0,
		Right,
		Middle,
		Button4,
		Button5,
		Count = 16
	};

	/**
	* @class Input
	* @brief Provides a static API for handling keyboard and mouse input.
	*
	* Offers functionality for querying the state of keyboard and mouse buttons,
	* as well as tracking mouse position and scroll deltas. It supports different key states such
	* as pressed, released, and held, making it suitable for a variety of input scenarios.
	*
	* This class is designed to be used globally as a singleton within an application to manage
	* input state across multiple windows. It does not require instantiation but instead requires
	* users to register the windows from which input should be captured.
	*
	* @see Input::registerWindow
	*/
	class Input {
	public:
		/**
		* @brief Register a window to listen for input events.
		*
		* Associates the given window with the input system, allowing it to process keyboard and mouse events.
		* This registration is required to enable input tracking for the specified window.
		*
		* @param window The window to be registered.
		*/
		RYN_API static void registerWindow(Window window);

		RYN_API static bool isKeyPressed(Key key);
		RYN_API static bool isKeyReleased(Key key);
		RYN_API static bool isKeyHeld(Key key);

		RYN_API static bool isMousePressed(MouseButton button);
		RYN_API static bool isMouseReleased(MouseButton button);
		RYN_API static bool isMouseHeld(MouseButton button);

		/**
		* @brief Retrieves the current mouse position.
		*
		* Returns the current mouse position in screen coordinates. The position is updated with the mouse's
		* x and y coordinates relative to the top-left corner of the window (top-left being 0,0).
		*
		* @param xPos A reference to store the current x-coordinate of the mouse.
		* @param yPos A reference to store the current y-coordinate of the mouse.
		*/
		RYN_API static void getMousePosition(double& xPos, double& yPos);

		/**
		* @brief Retrieves the scroll offset (delta) of the mouse wheel.
		*
		* Returns the horizontal and vertical scroll offsets accumulated since the last frame.
		*
		* @param xOffset A reference to store the horizontal scroll delta.
		* @param yOffset A reference to store the vertical scroll delta.
		*/
		RYN_API static void getMouseScroll(double& xOffset, double& yOffset);
	};
}