module;
#include <cstdint>

export module Ryntime.Core.Primitive:Keycodes;

export namespace ryn {
    enum class Keycode : uint8_t {
        // Unknown or unsupported key
        Unknown = 0,

        // Alphabetic keys
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,

        // Number keys (top row)
        D1,
        D2,
        D3,
        D4,
        D5,
        D6,
        D7,
        D8,
        D9,
        D0,

        // Function keys
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,

        // Arrow and navigation keys
        ArrowUp,
        ArrowDown,
        ArrowLeft,
        ArrowRight,

        // Control and symbol keys
        Escape,
        Tab,
        CapsLock,
        Space,
        Return,
        Backslash,
        Backspace,
        Minus,
        Equals,
        LBracket,
        RBracket,
        Semicolon,
        Apostrophe,
        Comma,
        Period,
        Slash,
        Insert,
        Delete,
        PageUp,
        PageDown,
        Home,
        Pause,
        End,
        ScrollLock,
        PrintScreen,
        NumLock,

        // Numeric keypad keys
        KP_Period,
        KP_Divide,
        KP_Multiply,
        KP_Minus,
        KP_Plus,
        KP_Enter,
        KP_1,
        KP_2,
        KP_3,
        KP_4,
        KP_5,
        KP_6,
        KP_7,
        KP_8,
        KP_9,
        KP_0,

        // Modifier keys
        LCtrl,
        LShift,
        LAlt,
        LSuper,
        RCtrl,
        RShift,
        RAlt,
        RSuper,

        // Highest possible keycode
        Count = 192
    };

    enum class MouseButton : uint8_t {
        // No button or invalid button
        Unknown = 0,

        // Standard mouse buttons
        Left,
        Middle,
        Right,

        // Auxiliary side buttons
        Side_1,
        Side_2,

        // Heighest possible mouse button
        Count = 32
    };
}
