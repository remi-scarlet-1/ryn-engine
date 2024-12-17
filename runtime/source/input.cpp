#include "input.h"

// Initialize static members:
uint64_t ryn::Input::regularKeyStates = 0;
uint64_t ryn::Input::regularKeyStatesLastFrame = 0;
uint64_t ryn::Input::specialKeyStates = 0;
uint64_t ryn::Input::specialKeyStatesLastFrame = 0;

uint16_t ryn::Input::mouseButtonStates = 0;
uint16_t ryn::Input::mouseButtonStatesLastFrame = 0;
double ryn::Input::mouseX = 0.0;
double ryn::Input::mouseY = 0.0;
double ryn::Input::mouseScrollX = 0.0;
double ryn::Input::mouseScrollY = 0.0;

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
bool ryn::Input::isKeyPressed(Key key) {
    int index = static_cast<int>(key);

    if (inRange(static_cast<int>(Key::A), static_cast<int>(Key::KeyCount), index))
        return getBit(regularKeyStates, index) && !getBit(regularKeyStatesLastFrame, index);
    else if (inRange(static_cast<int>(Key::LeftArrow), static_cast<int>(Key::ExtendedKeyCount), index)) {
        int adjustedIndex = index - static_cast<int>(Key::LeftArrow);
        return getBit(specialKeyStates, adjustedIndex) && !getBit(specialKeyStatesLastFrame, adjustedIndex);
    }

    return false;
}

bool ryn::Input::isKeyReleased(Key key) {
    int index = static_cast<int>(key);

    if (inRange(static_cast<int>(Key::A), static_cast<int>(Key::KeyCount), index))
        return !getBit(regularKeyStates, index) && getBit(regularKeyStatesLastFrame, index);
    else if (inRange(static_cast<int>(Key::LeftArrow), static_cast<int>(Key::ExtendedKeyCount), index)) {
        int adjustedIndex = index - static_cast<int>(Key::LeftArrow);
        return !getBit(specialKeyStates, adjustedIndex) && getBit(specialKeyStatesLastFrame, adjustedIndex);
    }

    return false;
}

bool ryn::Input::isKeyHeld(Key key) {
    int index = static_cast<int>(key);

    if (inRange(static_cast<int>(Key::A), static_cast<int>(Key::KeyCount), index))
        return getBit(regularKeyStates, index);
    else if (inRange(static_cast<int>(Key::LeftArrow), static_cast<int>(Key::ExtendedKeyCount), index)) {
        int adjustedIndex = index - static_cast<int>(Key::LeftArrow);
        return getBit(specialKeyStates, adjustedIndex);
    }

    return false;
}

bool ryn::Input::isMousePressed(MouseButton button) {
    int index = static_cast<int>(button);
    if (index < static_cast<int>(MouseButton::Count))
        return getBit(mouseButtonStates, index) && !getBit(mouseButtonStatesLastFrame, index);
    return false;
}

bool ryn::Input::isMouseReleased(MouseButton button) {
    int index = static_cast<int>(button);
    if (index < static_cast<int>(MouseButton::Count))
        return !getBit(mouseButtonStates, index) && getBit(mouseButtonStatesLastFrame, index);
    return false;
}

bool ryn::Input::isMouseHeld(MouseButton button) {
    int index = static_cast<int>(button);
    if (index < static_cast<int>(MouseButton::Count))
        return getBit(mouseButtonStates, index);
    return false;
}

void ryn::Input::getMousePosition(double& xPos, double& yPos) {
    xPos = mouseX;
    yPos = mouseY;
}

void ryn::Input::getMouseScroll(double& xOffset, double& yOffset) {
    xOffset = mouseScrollX;
    yOffset = mouseScrollY;
}

// Internal functions
void ryn::Input::reset() { // TODO: FIX!!!!
    regularKeyStatesLastFrame = regularKeyStates;
    specialKeyStatesLastFrame = specialKeyStates;
    mouseButtonStatesLastFrame = mouseButtonStates;
}

void ryn::Input::updateKeyState(Key key, bool pressed) {
    int index = static_cast<int>(key);
    if (inRange(static_cast<int>(Key::A), static_cast<int>(Key::KeyCount), index))
        return setBit(regularKeyStates, index, pressed);
    else if (inRange(static_cast<int>(Key::LeftArrow), static_cast<int>(Key::ExtendedKeyCount), index)) {
        int adjustedIndex = index - static_cast<int>(Key::LeftArrow);
        return setBit(specialKeyStates, adjustedIndex, pressed);
    }
}

void ryn::Input::updateMouseButtonState(MouseButton button, bool pressed) {
    int index = static_cast<int>(button);
    if (index < static_cast<int>(MouseButton::Count))
        setBit(mouseButtonStates, index, pressed);
}

void ryn::Input::updateMousePosition(double xPos, double yPos) {
    mouseX = xPos;
    mouseY = yPos;
}

void ryn::Input::updateMouseScroll(double xOffset, double yOffset) {
    mouseScrollX = xOffset;
    mouseScrollY = yOffset;
}