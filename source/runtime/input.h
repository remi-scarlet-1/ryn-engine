/**
* @file    input.h
* @brief   Provides a static inteface for input handling of devices such as keyboards and mice. 
*          This file allows users to query input states, track mouse movements and scroll.
* @author  Konstantinos Lampros
* @version 1.0.0
* @date    2025-01-12
*/

#pragma once

#include "defines.h"

// Forward declarations
struct GLFWwindow;

namespace ryn::window {
    class Window;
}

namespace ryn::input {
    /**
    * @enum Key
    * @brief Represents all possible keys on the keyboard.
    *
    * Defines all keys that can be monitored by the input system.
    * It includes standard alphanumeric keys (A-Z, 0-9), function keys (F1-F12), arrow keys,
    * and special keys such as Enter, Escape, Shift, etc.
    *
    * @note The enum values are internally mapped to GLFW key codes.
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
    * @enum MouseButton
    * @brief Represents all buttons on the mouse.
    *
    * Defines all buttons on the mouse that can be monitored by the input system.
    * It includes typical mouse buttons like left, right, middle (scroll wheel), and additional
    * buttons such as Button4 and Button5 commonly found on gaming mice (side buttons).
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
    class RYN_API Input {
    public:
        /**
        * @brief Prepares the input system for a new frame by resetting its state.
        *
        * Updates internal state tracking by copying the current frame's input state
        * (keys and mouse buttons) into containers that store the previous frame's state.
        * This allows queries about whether inputs were pressed, released, or held during the frame.
        * 
        * @note This method should be called at the beginning of each frame, prior to polling events.
        */
        static void reset();

        /**
        * @brief Register a window to listen for input events.
        * 
        * Associates the given window with the input system, allowing it to process keyboard and mouse events. 
        * This registration is required to enable input tracking for the specified window.
        * 
        * @param window The window to be registered.
        */
        static void registerWindow(window::Window* window);

		static bool isKeyPressed(Key key);
		static bool isKeyReleased(Key key);
		static bool isKeyHeld(Key key);

		static bool isMousePressed(MouseButton button);
		static bool isMouseReleased(MouseButton button);
		static bool isMouseHeld(MouseButton button);

        /**
        * @brief Retrieves the current mouse position.
        * 
        * Returns the current mouse position in screen coordinates. The position is updated with the mouse's
        * x and y coordinates relative to the top-left corner of the window (top-left being 0,0).
        * 
        * @param xPos A reference to store the current x-coordinate of the mouse.
        * @param yPos A reference to store the current y-coordinate of the mouse.
        */
		static void getMousePosition(double& xPos, double& yPos);

        /**
        * @brief Retrieves the scroll offset (delta) of the mouse wheel.
        * 
        * Returns the horizontal and vertical scroll offsets accumulated since the last frame.
        * 
        * @param xOffset A reference to store the horizontal scroll delta.
        * @param yOffset A reference to store the vertical scroll delta.
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

        // Doubles storing other mouse information
        static double mouseX;
        static double mouseY;
        static double mouseScrollX;
        static double mouseScrollY;

        // Internal functions used to update states
		static void updateKeyState(Key key, bool pressed);
		static void updateMouseButtonState(MouseButton button, bool pressed);
		static void updateMousePosition(double xPos, double yPos);
		static void updateMouseScroll(double xOffset, double yOffset);

        // Callbacks used in conjunction with the windowing backend (GLFW)
        // Make use of the input systems internal update functions and map GLFW specific keycodes to input system ones
		static void onKey(GLFWwindow* win, int key, int scancode, int action, int mods);
		static void onMouseButton(GLFWwindow* win, int button, int action, int mods);
		static void onMouseScroll(GLFWwindow* win, double xOffset, double yOffset);
		static void onMousePosition(GLFWwindow* window, double xPos, double yPos);
	};
}