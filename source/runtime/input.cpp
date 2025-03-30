#include "input.h"
#include "window.h"
#include <GLFW/glfw3.h>

using namespace ryn::input;

uint64_t Input::regularKeyStates = 0;
uint64_t Input::regularKeyStatesLastFrame = 0;
uint64_t Input::specialKeyStates = 0;
uint64_t Input::specialKeyStatesLastFrame = 0;

uint16_t Input::mouseButtonStates = 0;
uint16_t Input::mouseButtonStatesLastFrame = 0;
double Input::mouseX = 0.0;
double Input::mouseY = 0.0;
double Input::mouseScrollX = 0.0;
double Input::mouseScrollY = 0.0;

#pragma region Bitwise manipulation helper functions
/**
 * @brief Sets or clears a specific bit in a 64-bit word.
 *
 * @param word The 64-bit word to modify.
 * @param bit The bit index to set or clear (0-based).
 * @param value If true, the bit is set to 1; if false, the bit is cleared to 0.
 */
inline void setBit(uint64_t& word, int bit, bool value) {
    if (value)
        word |= (1ULL << bit);
    else
        word &= ~(1ULL << bit);
}

/**
 * @brief Sets or clears a specific bit in a 16-bit word.
 *
 * @param word The 16-bit word to modify.
 * @param bit The bit index to set or clear (0-based).
 * @param value If true, the bit is set to 1; if false, the bit is cleared to 0.
 */
inline void setBit(uint16_t& word, int bit, bool value) {
    if (value)
        word |= (1ULL << bit);
    else
        word &= ~(1ULL << bit);
}

/**
 * @brief Checks whether a specific bit is set in a 64-bit word.
 *
 * @param word The 64-bit word to check.
 * @param bit The bit index to check (0-based).
 * @return true if the bit is set (1), false otherwise.
 */
inline constexpr bool getBit(const uint64_t& word, int bit) {
    return word & (1ULL << bit);
}
#pragma endregion

void Input::reset() {
    regularKeyStatesLastFrame = regularKeyStates;
    specialKeyStatesLastFrame = specialKeyStates;
    mouseButtonStatesLastFrame = mouseButtonStates;
    mouseScrollX = 0;
    mouseScrollY = 0;
}

void Input::registerWindow(window::Window* window) {
    GLFWwindow* glfwWindow = window->getGlfwHandle();
    glfwSetKeyCallback(glfwWindow, onKey);
    glfwSetMouseButtonCallback(glfwWindow, onMouseButton);
    glfwSetScrollCallback(glfwWindow, onMouseScroll);
    glfwSetCursorPosCallback(glfwWindow, onMousePosition);
}

bool Input::isKeyPressed(Key key) {
    int index = static_cast<int>(key);

    if (index < static_cast<int>(Key::KeyCount))
        return getBit(regularKeyStates, index) && !getBit(regularKeyStatesLastFrame, index);
    else {
        int adjustedIndex = index - static_cast<int>(Key::LeftArrow);
        return getBit(specialKeyStates, adjustedIndex) && !getBit(specialKeyStatesLastFrame, adjustedIndex);
    }
}

bool Input::isKeyReleased(Key key) {
    int index = static_cast<int>(key);

    if (index < static_cast<int>(Key::KeyCount))
        return !getBit(regularKeyStates, index) && getBit(regularKeyStatesLastFrame, index);
    else {
        int adjustedIndex = index - static_cast<int>(Key::LeftArrow);
        return !getBit(specialKeyStates, adjustedIndex) && getBit(specialKeyStatesLastFrame, adjustedIndex);
    }
}

bool Input::isKeyHeld(Key key) {
    int index = static_cast<int>(key);

    if (index < static_cast<int>(Key::KeyCount))
        return getBit(regularKeyStates, index);
    else {
        int adjustedIndex = index - static_cast<int>(Key::LeftArrow);
        return getBit(specialKeyStates, adjustedIndex);
    }
}

bool Input::isMousePressed(MouseButton button) {
    int index = static_cast<int>(button);
    return getBit(mouseButtonStates, index) && !getBit(mouseButtonStatesLastFrame, index);
}

bool Input::isMouseReleased(MouseButton button) {
    int index = static_cast<int>(button);
    return !getBit(mouseButtonStates, index) && getBit(mouseButtonStatesLastFrame, index);
}

bool Input::isMouseHeld(MouseButton button) {
    int index = static_cast<int>(button);
    return getBit(mouseButtonStates, index);
}

void Input::getMousePosition(double& xPos, double& yPos) {
    xPos = mouseX;
    yPos = mouseY;
}

void Input::getMouseScroll(double& xOffset, double& yOffset) {
    xOffset = mouseScrollX;
    yOffset = mouseScrollY;
}

void Input::updateKeyState(Key key, bool pressed) {
    int index = static_cast<int>(key);
    if (index < static_cast<int>(Key::KeyCount))
        return setBit(regularKeyStates, index, pressed);
    else {
        int adjustedIndex = index - static_cast<int>(Key::LeftArrow);
        return setBit(specialKeyStates, adjustedIndex, pressed);
    }
}

void Input::updateMouseButtonState(MouseButton button, bool pressed) {
    int index = static_cast<int>(button);
    if (index < static_cast<int>(MouseButton::Count))
        setBit(mouseButtonStates, index, pressed);
}

void Input::updateMousePosition(double xPos, double yPos) {
    mouseX = xPos;
    mouseY = yPos;
}

void Input::updateMouseScroll(double xOffset, double yOffset) {
    mouseScrollX = xOffset;
    mouseScrollY = yOffset;
}

void Input::onKey(GLFWwindow* win, int key, int scancode, int action, int mods) {
    Key mappedKey;

    if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z)
        mappedKey = static_cast<Key>(key - GLFW_KEY_A);
    else if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9)
        mappedKey = static_cast<Key>(key);
    else if (key >= GLFW_KEY_F1 && key <= GLFW_KEY_F12)
        mappedKey = static_cast<Key>(key - GLFW_KEY_F1 + static_cast<int>(Key::F1));
    else {
        switch (key) {
        case GLFW_KEY_LEFT:             mappedKey = Key::LeftArrow;     break;
        case GLFW_KEY_RIGHT:            mappedKey = Key::RightArrow;    break;
        case GLFW_KEY_UP:               mappedKey = Key::UpArrow;       break;
        case GLFW_KEY_DOWN:             mappedKey = Key::DownArrow;     break;

        case GLFW_KEY_ENTER:            mappedKey = Key::Enter;         break;
        case GLFW_KEY_SPACE:            mappedKey = Key::Space;         break;
        case GLFW_KEY_TAB:              mappedKey = Key::Tab;           break;
        case GLFW_KEY_ESCAPE:           mappedKey = Key::Escape;        break;
        case GLFW_KEY_BACKSPACE:        mappedKey = Key::Backspace;     break;
        case GLFW_KEY_LEFT_SHIFT:       mappedKey = Key::Shift;         break;
        case GLFW_KEY_RIGHT_SHIFT:      mappedKey = Key::Shift;         break;
        case GLFW_KEY_LEFT_CONTROL:     mappedKey = Key::Control;       break;
        case GLFW_KEY_RIGHT_CONTROL:    mappedKey = Key::Control;       break;
        case GLFW_KEY_LEFT_ALT:         mappedKey = Key::Alt;           break;
        case GLFW_KEY_RIGHT_ALT:        mappedKey = Key::Alt;           break;
        case GLFW_KEY_CAPS_LOCK:        mappedKey = Key::CapsLock;      break;
        case GLFW_KEY_NUM_LOCK:         mappedKey = Key::NumLock;       break;
        case GLFW_KEY_SCROLL_LOCK:      mappedKey = Key::ScrollLock;    break;
        case GLFW_KEY_INSERT:           mappedKey = Key::Insert;        break;
        case GLFW_KEY_DELETE:           mappedKey = Key::Delete;        break;

        case GLFW_KEY_HOME:             mappedKey = Key::Home;          break;
        case GLFW_KEY_END:              mappedKey = Key::End;           break;
        case GLFW_KEY_PAGE_UP:          mappedKey = Key::PageUp;        break;
        case GLFW_KEY_PAGE_DOWN:        mappedKey = Key::PageDown;      break;
        case GLFW_KEY_PAUSE:            mappedKey = Key::Pause;         break;
        case GLFW_KEY_PRINT_SCREEN:     mappedKey = Key::PrintScreen;   break;

        case GLFW_KEY_MINUS:            mappedKey = Key::Minus;         break;
        case GLFW_KEY_EQUAL:            mappedKey = Key::Equal;         break;
        case GLFW_KEY_LEFT_BRACKET:     mappedKey = Key::LeftBracket;   break;
        case GLFW_KEY_RIGHT_BRACKET:    mappedKey = Key::RightBracket;  break;
        case GLFW_KEY_BACKSLASH:        mappedKey = Key::Backslash;     break;
        case GLFW_KEY_SEMICOLON:        mappedKey = Key::Semicolon;     break;
        case GLFW_KEY_APOSTROPHE:       mappedKey = Key::Apostrophe;    break;
        case GLFW_KEY_COMMA:            mappedKey = Key::Comma;         break;
        case GLFW_KEY_PERIOD:           mappedKey = Key::Period;        break;
        case GLFW_KEY_SLASH:            mappedKey = Key::Slash;         break;
        default: return;
        }
    }

    updateKeyState(mappedKey, action != GLFW_RELEASE);
}

void Input::onMouseButton(GLFWwindow* win, int button, int action, int mods) {
    MouseButton mappedButton;

    switch (button) {
    case GLFW_MOUSE_BUTTON_LEFT:    mappedButton = MouseButton::Left;       break;
    case GLFW_MOUSE_BUTTON_RIGHT:   mappedButton = MouseButton::Right;      break;
    case GLFW_MOUSE_BUTTON_MIDDLE:  mappedButton = MouseButton::Middle;     break;
    case GLFW_MOUSE_BUTTON_4:       mappedButton = MouseButton::Button4;    break;
    case GLFW_MOUSE_BUTTON_5:       mappedButton = MouseButton::Button5;    break;
    default: return;
    }

    updateMouseButtonState(mappedButton, action != GLFW_RELEASE);
}

void Input::onMouseScroll(GLFWwindow* win, double xOffset, double yOffset) {
    updateMouseScroll(xOffset, yOffset);
}

void Input::onMousePosition(GLFWwindow* window, double xPos, double yPos) {
    updateMousePosition(xPos, yPos);
}
