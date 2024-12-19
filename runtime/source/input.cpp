#include "input.h"

using namespace ryn;

// Initialize static members
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

// Helper functions
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

// Implementation
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

// Internal functions
void Input::reset() {
    regularKeyStatesLastFrame = regularKeyStates;
    specialKeyStatesLastFrame = specialKeyStates;
    mouseButtonStatesLastFrame = mouseButtonStates;
    mouseScrollX = 0;
    mouseScrollY = 0;
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