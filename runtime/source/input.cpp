#include "input.h"

#include <bitset>

// Initialize static members:
uint8_t ryn::Input::modifierState = 0;

std::bitset<static_cast<int>(ryn::Key::Count)> ryn::Input::keyStates;
std::bitset<static_cast<int>(ryn::Key::Count)> ryn::Input::keyStatesLastFrame;

std::bitset<static_cast<int>(ryn::MouseButton::Count)> ryn::Input::mouseButtonStates;
std::bitset<static_cast<int>(ryn::MouseButton::Count)> ryn::Input::mouseButtonStatesLastFrame;

double ryn::Input::mouseX = 0.0;
double ryn::Input::mouseY = 0.0;

double ryn::Input::mouseScrollX = 0.0;
double ryn::Input::mouseScrollY = 0.0;

// Function implementations:
bool ryn::Input::isKeyPressed(Key key, uint8_t modifiers) {
    int index = static_cast<int>(key);
    if (index < static_cast<int>(Key::Count)) // Ensure key index is valid
        if (keyStatesLastFrame.test(index) == false && keyStates.test(index) == true) // Check if pressed this frame
            if ((modifierState & modifiers) == modifiers) // Check if required modifiers are set
                return true;
    return false;
}

bool ryn::Input::isKeyReleased(Key key, uint8_t modifiers) {
    int index = static_cast<int>(key);
    if (index < static_cast<int>(Key::Count)) // Ensure key index is valid
        if (keyStatesLastFrame.test(index) == true && keyStates.test(index) == false) // Check if released this frame
            if ((modifierState & modifiers) == modifiers) // Check if required modifiers are set
                return true;
    return false;
}

bool ryn::Input::isKeyHeld(Key key, uint8_t modifiers) {
    int index = static_cast<int>(key);
    if (index < static_cast<int>(Key::Count)) // Ensure key index is valid
        if (keyStates.test(index) == true) // Check if key is held
            if ((modifierState & modifiers) == modifiers) // Check if required modifiers are set
                return true;
    return false;
}

bool ryn::Input::isMousePressed(MouseButton button) {
    int index = static_cast<int>(button);
    if (index < static_cast<int>(MouseButton::Count)) // Ensure mouse button index is valid
        if (mouseButtonStatesLastFrame.test(index) == false && mouseButtonStates.test(index) == true) // Check if pressed this frame
            return true;
    return false;
}

bool ryn::Input::isMouseReleased(MouseButton button) {
    int index = static_cast<int>(button);
    if (index < static_cast<int>(MouseButton::Count)) // Ensure mouse button index is valid
        if (mouseButtonStatesLastFrame.test(index) == true && mouseButtonStates.test(index) == false) // Check if released this frame
            return true;
    return false;
}

bool ryn::Input::isMouseHeld(MouseButton button) {
    int index = static_cast<int>(button);
    if (index < static_cast<int>(MouseButton::Count)) // Ensure mouse button index is valid
        if (mouseButtonStates.test(index) == true) // Check if button is held
            return true;
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

// Internal functions:
void ryn::Input::update() {
    keyStatesLastFrame = keyStates;
    keyStates.reset();

    mouseButtonStatesLastFrame = mouseButtonStates;
    mouseButtonStates.reset();
}

void ryn::Input::updateKeyState(Key key, bool pressed) {
    int index = static_cast<int>(key);
    if (index < static_cast<int>(Key::Count))
        keyStates.set(index, pressed);
}

void ryn::Input::updateModifierState(Modifier mod, bool pressed) {
    uint8_t bit = static_cast<uint8_t>(mod);
    if (pressed)
        modifierState |= bit;
    else
        modifierState &= ~bit;
}

void ryn::Input::updateMouseButtonState(MouseButton button, bool pressed) {
    int index = static_cast<int>(button);
    if (index < static_cast<int>(MouseButton::Count))
        mouseButtonStates.set(index, pressed);
}

void ryn::Input::updateMousePosition(double xPos, double yPos) {
    mouseX = xPos;
    mouseY = yPos;
}

void ryn::Input::updateMouseScroll(double xOffset, double yOffset) {
    mouseScrollX = xOffset;
    mouseScrollY = yOffset;
}