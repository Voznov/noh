#include "ControlHandler.h"

#include <algorithm>

#include <GLFW/glfw3.h>

ControlHandler::ControlHandler(GLFWwindow* window) : window(window)
{

    std::fill(keyboardIsPressed, keyboardIsPressed + _countof(keyboardIsPressed), false);
    std::fill(keyboardHandles, keyboardHandles + _countof(keyboardHandles), nullptr);

    std::fill(mouseButtonIsPressed, mouseButtonIsPressed + _countof(mouseButtonIsPressed), false);
    std::fill(mouseButtonHandles, mouseButtonHandles + _countof(mouseButtonHandles), nullptr);

    updateMousePosition();
}
void ControlHandler::keyboardCallback(int key, int scancode, int action, int mods) {
    _buttonCallback(keyboardIsPressed, keyboardHandles, key, action, mods);
}

void ControlHandler::mousePositionCallback(double x, double y)
{
    mousePosition.lastX = mousePosition.currentX;
    mousePosition.lastY = mousePosition.currentY;

    mousePosition.currentX = x;
    mousePosition.currentY = y;

    mousePosition.diffX = mousePosition.currentX - mousePosition.lastX;
    mousePosition.diffY = mousePosition.currentY - mousePosition.lastY;
}

void ControlHandler::mouseButtonCallback(int button, int action, int mods)
{
    _buttonCallback(mouseButtonIsPressed, mouseButtonHandles, button, action, mods);
}

void ControlHandler::clearMousePositionDiff()
{
    mousePositionCallback(mousePosition.currentX, mousePosition.currentY);
}

void ControlHandler::updateMousePosition()
{
    glfwGetCursorPos(window, &mousePosition.currentX, &mousePosition.currentY);
    clearMousePositionDiff();
}

void ControlHandler::_buttonCallback(bool* isPressed, ButtonHandle* handles, int button, int action, int mods)
{
    if (button == GLFW_KEY_UNKNOWN)
        return;

    if (action == GLFW_PRESS) {
        isPressed[button] = true;
        if (handles[button])
            handles[button](button, action);
        return;
    }

    if (action == GLFW_RELEASE) {
        isPressed[button] = false;
        return;
    }
}
