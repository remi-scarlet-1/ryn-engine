#pragma once

// Forward declaration of std::bitset with a specified size
namespace std {
	template <size_t N> class bitset;
}

// Engine namespace
namespace ryn {
	// Forward declaration of ryn::Window class
	class Window;
	
	// Provides a static API for querying inputs
	class Input {
	public:
		// Modifier keys (Shift, Ctrl, Alt)
		enum class Modifier {
			None = 0x0,    // 0000 (no modifier)
			Shift = 0x1,   // 0001
			Control = 0x2, // 0010
			Alt = 0x4      // 0100
		};

		// Keyboard keys
		enum class Key {
			A = 0, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
			Zero = 48, One, Two, Three, Four, Five, Six, Seven, Eight, Nine,
			F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
			LeftArrow, RightArrow, UpArrow, DownArrow,
			Enter, Space, Tab, Escape, Backspace, Shift, Control, Alt, CapsLock, NumLock, ScrollLock, Insert, Delete,
			Home, End, PageUp, PageDown, Pause, PrintScreen,
			Count // Placeholder for querying total number of keys
		};

		// Mouse buttons
		enum class MouseButton {
			Left = 0,
			Right,
			Middle,
			Count // Placeholder for querying total mouse buttons
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
		friend class Window; // Allow window class to access update functions of Input API

		// Internal state
		static std::bitset<static_cast<int>(Key::Count)> keyStates;
		static std::bitset<static_cast<int>(Key::Count)> keyStatesLastFrame;
		static std::bitset<static_cast<int>(MouseButton::Count)> mouseButtonStates;
		static std::bitset<static_cast<int>(MouseButton::Count)> mouseButtonStatesLastFrame;
		static double mouseX, mouseY;
		static double mouseScrollX, mouseScrollY;

		// Update functions to be used be used by window class:
		
		// Called at the start of a new frame. 
		// Updates lastFrame states and prepares current states for new input.
		static void update();
		static void updateKeyState(Key key, bool pressed);
		static void updateMouseButtonState(MouseButton button, bool pressed);
		static void updateMousePosition(double xPos, double yPos);
		static void updateMouseScroll(double xOffset, double yOffset);
	};
}