#include "input.h"

using namespace ryn;

// Initialize static members:
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
inline void setBit(uint64_t& word, int bit, bool value) {
    if (value)
        word |= (1ULL << bit);
    else
        word &= ~(1ULL << bit);
}

inline void setBit(uint16_t& word, int bit, bool value) {
    if (value)
        word |= (1ULL << bit);
    else
        word &= ~(1ULL << bit);
}

inline constexpr bool getBit(const uint64_t& word, int bit) {
    return word & (1ULL << bit);
}

inline constexpr bool inRange(int start, int end, int a) {
    return start <= a && a < end;
}

// Implementation
bool Input::isKeyPressed(Key key) {
    int index = static_cast<int>(key);

    if (inRange(static_cast<int>(Key::A), static_cast<int>(Key::KeyCount), index))
        return getBit(regularKeyStates, index) && !getBit(regularKeyStatesLastFrame, index);
    else if (inRange(static_cast<int>(Key::LeftArrow), static_cast<int>(Key::ExtendedKeyCount), index)) {
        int adjustedIndex = index - static_cast<int>(Key::LeftArrow);
        return getBit(specialKeyStates, adjustedIndex) && !getBit(specialKeyStatesLastFrame, adjustedIndex);
    }

    return false;
}

bool Input::isKeyReleased(Key key) {
    int index = static_cast<int>(key);

    if (inRange(static_cast<int>(Key::A), static_cast<int>(Key::KeyCount), index))
        return !getBit(regularKeyStates, index) && getBit(regularKeyStatesLastFrame, index);
    else if (inRange(static_cast<int>(Key::LeftArrow), static_cast<int>(Key::ExtendedKeyCount), index)) {
        int adjustedIndex = index - static_cast<int>(Key::LeftArrow);
        return !getBit(specialKeyStates, adjustedIndex) && getBit(specialKeyStatesLastFrame, adjustedIndex);
    }

    return false;
}

bool Input::isKeyHeld(Key key) {
    int index = static_cast<int>(key);

    if (inRange(static_cast<int>(Key::A), static_cast<int>(Key::KeyCount), index))
        return getBit(regularKeyStates, index);
    else if (inRange(static_cast<int>(Key::LeftArrow), static_cast<int>(Key::ExtendedKeyCount), index)) {
        int adjustedIndex = index - static_cast<int>(Key::LeftArrow);
        return getBit(specialKeyStates, adjustedIndex);
    }

    return false;
}

bool Input::isMousePressed(MouseButton button) {
    int index = static_cast<int>(button);
    if (index < static_cast<int>(MouseButton::Count))
        return getBit(mouseButtonStates, index) && !getBit(mouseButtonStatesLastFrame, index);
    return false;
}

bool Input::isMouseReleased(MouseButton button) {
    int index = static_cast<int>(button);
    if (index < static_cast<int>(MouseButton::Count))
        return !getBit(mouseButtonStates, index) && getBit(mouseButtonStatesLastFrame, index);
    return false;
}

bool Input::isMouseHeld(MouseButton button) {
    int index = static_cast<int>(button);
    if (index < static_cast<int>(MouseButton::Count))
        return getBit(mouseButtonStates, index);
    return false;
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
void Input::reset() { // TODO: FIX!!!!
    regularKeyStatesLastFrame = regularKeyStates;
    specialKeyStatesLastFrame = specialKeyStates;
    mouseButtonStatesLastFrame = mouseButtonStates;
}

void Input::updateKeyState(Key key, bool pressed) {
    int index = static_cast<int>(key);
    if (inRange(static_cast<int>(Key::A), static_cast<int>(Key::KeyCount), index))
        return setBit(regularKeyStates, index, pressed);
    else if (inRange(static_cast<int>(Key::LeftArrow), static_cast<int>(Key::ExtendedKeyCount), index)) {
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