#pragma once

#include "public/defines.h"
#include "window.h"

struct GLFWwindow;

namespace ryn {
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

		static void reset();
		RYN_API static void registerWindow(Window window);

		RYN_API static bool isKeyPressed(Key key);
		RYN_API static bool isKeyReleased(Key key);
		RYN_API static bool isKeyHeld(Key key);

		RYN_API static bool isMousePressed(MouseButton button);
		RYN_API static bool isMouseReleased(MouseButton button);
		RYN_API static bool isMouseHeld(MouseButton button);

		RYN_API static void getMousePosition(double& xPos, double& yPos);
		RYN_API static void getMouseScroll(double& xOffset, double& yOffset);

	private:
		static uint64_t regularKeyStates;
		static uint64_t regularKeyStatesLastFrame;
		static uint64_t specialKeyStates;
		static uint64_t specialKeyStatesLastFrame;

		static uint16_t mouseButtonStates;
		static uint16_t mouseButtonStatesLastFrame;
		static double mouseX, mouseY;
		static double mouseScrollX, mouseScrollY;

		static void updateKeyState(Key key, bool pressed);
		static void updateMouseButtonState(MouseButton button, bool pressed);
		static void updateMousePosition(double xPos, double yPos);
		static void updateMouseScroll(double xOffset, double yOffset);

		static void onKey(GLFWwindow* win, int key, int scancode, int action, int mods);
		static void onMouseButton(GLFWwindow* win, int button, int action, int mods);
		static void onMouseScroll(GLFWwindow* win, double xOffset, double yOffset);
		static void onMousePosition(GLFWwindow* window, double xPos, double yPos);
	};
}