/**
 * @file        input.h
 * @brief       Provides a static API for input management.
 * 
 * @warning     Ensure to register windows through `ryn::Input::registerWindow(...)` such that
 *              inputs while focused on that window are registered and managed.
 * 
 * @author      Konstantinos Lampros
 * @version     1.0.0
 * @date        2025-01-12
 */

#pragma once

#include "ryn/defines.h"

struct GLFWwindow;

namespace ryn {
    class Window;
}

namespace ryn {
    /**
	 * @enum    Key
	 * @brief   Represents all supported keyboard keycodes. Includes standard alphanumeric (A-Z, 0-9),
     *          function keys (F1-F12), arrow keys and special keys such as Enter, Escape, Shift, etc.
	 * @note    Enum values are mapped internally to GLFW keycodes.
	 */
    enum class RYN_API Key {
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
	 * @enum    MouseButton
	 * @brief   Represents all supported mouse buttons. Includes typical mouse buttons like left, right,
     *          middle, and additional buttons such a Button4 and Button5 commonly found on gaming mice.
	 */
    enum class RYN_API MouseButton {
        Left = 0,
        Right,
        Middle,
        Button4,
        Button5,
        Count = 16
    };

    /**
	 * @class   Input
	 * @brief   Provides a static API for handling querying input state. This class is designed to be used globally
	 *          as a singleton within an application to manage input state across multiple windows. It does not require
	 *          instantiation but instead requires users to register the windows from which input should be captured via
	 *          `ryn::Input::registerWindow(...)`.
     */
    class RYN_API Input {
    public:
        /**
		 * @brief   Prepares the input system for a new frame by resetting its state. Updates internal state tracking by
		 *          copying the current frame's input state into containers that store the previous frame's state. This
		 *          allows queries about whether inputs were pressed, released, or held during the frame.
		 *
		 * @warning This method must be called at the beginning of each frame, prior to polling events.
         */
        static void reset();

        /**
		 * @brief   Register a window to capture its input events into global state. This registration is required to enable
		 *          input tracking for the specified window.
		 * @param   window: A pointer to the window to be registered.
		 */
        static void registerWindow(Window* window);

		static bool isKeyPressed(Key key);
		static bool isKeyReleased(Key key);
		static bool isKeyHeld(Key key);

		static bool isMousePressed(MouseButton button);
		static bool isMouseReleased(MouseButton button);
		static bool isMouseHeld(MouseButton button);

        /**
		 * @brief   Retrieves the mouse position in window screen coordinates (top-left being 0,0).
		 * @param   xPos: A reference to store the x-coordinate of the mouse.
		 * @param   yPos: A reference to store the y-coordinate of the mouse.
		 */
		static void getMousePosition(double& xPos, double& yPos);

        /**
		 * @brief    Retrieves the mouse wheel scroll offset (delta).
		 * @param    xOffset: A reference to store the horizontal scroll delta.
		 * @param    yOffset: A reference to store the vertical scroll delta.
		 */
		static void getMouseScroll(double& xOffset, double& yOffset);

	private:
        // 64-bit integers used as bitmaps for keys
        // Store pressed state as 1 and released state as 0
		static uint64_t regularKeyStates;
		static uint64_t regularKeyStatesLastFrame;
		static uint64_t specialKeyStates;
		static uint64_t specialKeyStatesLastFrame;

        // 16-bit integers used as bitmaps for mouse buttons
        // Store pressed state as 1 and released state as 0
		static uint16_t mouseButtonStates;
		static uint16_t mouseButtonStatesLastFrame;

        // Further mouse input state
        static double mouseX;
        static double mouseY;
        static double mouseScrollX;
        static double mouseScrollY;

        // Internal functions used to update states
		static void updateKeyState(Key key, bool pressed);
		static void updateMouseButtonState(MouseButton button, bool pressed);
		static void updateMousePosition(double xPos, double yPos);
		static void updateMouseScroll(double xOffset, double yOffset);

        // Callbacks used in conjunction with the windowing back-end (GLFW).
		// These map GLFW keycodes to input system ones and update global input state using the systems internal functions.
		static void onKey(GLFWwindow* win, int key, int scancode, int action, int mods);
		static void onMouseButton(GLFWwindow* win, int button, int action, int mods);
		static void onMouseScroll(GLFWwindow* win, double xOffset, double yOffset);
		static void onMousePosition(GLFWwindow* window, double xPos, double yPos);
	};
}