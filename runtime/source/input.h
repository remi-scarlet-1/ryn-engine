#pragma once

#include <cstdint> // For uint64_t, uint16_t

// Engine namespace
namespace ryn {
	// Forward declarations
	class Window;
	class Debug;
	
	// Provides an interface for querying input states, including keyboard and mouse interactions.
	// This API is designed to aggregate input updates over a frame, allowing consistent polling.
	class Input {
	public:
		// Keyboard keys
		enum class Key {
			// Regular keys
			A = 0, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
			Zero, One, Two, Three, Four, Five, Six, Seven, Eight, Nine,
			F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
			KeyCount = 63,

			// Special keys
			LeftArrow = 64, RightArrow, UpArrow, DownArrow,
			Enter, Space, Tab, Escape, Backspace, Shift, Control, Alt, CapsLock, NumLock, ScrollLock,
			Insert, Delete, Home, End, PageUp, PageDown, Pause, PrintScreen,
			Minus, Equal, LeftBracket, RightBracket, Backslash, Semicolon, Apostrophe, Comma, Period, Slash,
			ExtendedKeyCount = 127
		};

		// Mouse buttons
		enum class MouseButton {
			Left = 0,  // Left mouse button
			Right,     // Right mouse button
			Middle,    // Middle mouse button (scroll wheel button)
			Button4,   // Extra mouse button (commonly side button 1)
			Button5,   // Extra mouse button (commonly side button 2)
			Count = 16 // Supports up to 16 mouse buttons
		};

		static bool isKeyPressed(Key key);
		static bool isKeyReleased(Key key);
		static bool isKeyHeld(Key key); 

		static bool isMousePressed(MouseButton button);
		static bool isMouseReleased(MouseButton button);
		static bool isMouseHeld(MouseButton button);
		
		static void getMousePosition(double& xPos, double& yPos);
		static void getMouseScroll(double& xOffset, double& yOffset);

	private:
		friend class Window; // Allow window class access to update functions of Input API
		friend class Debug; // Allow debug class access to retrieve internal state of the Input API

		// Internal state
		static uint64_t regularKeyStates;
		static uint64_t regularKeyStatesLastFrame;
		static uint64_t specialKeyStates;
		static uint64_t specialKeyStatesLastFrame;

		static uint16_t mouseButtonStates;
		static uint16_t mouseButtonStatesLastFrame;
		static double mouseX, mouseY;
		static double mouseScrollX, mouseScrollY;

		// Call this at the start of each frame, before the windowing library polls input.
		// Failing to do so may result in incorrect input states for the current frame.
		static void reset();
		static void updateKeyState(Key key, bool pressed);
		static void updateMouseButtonState(MouseButton button, bool pressed);
		static void updateMousePosition(double xPos, double yPos);
		static void updateMouseScroll(double xOffset, double yOffset);
	};
}