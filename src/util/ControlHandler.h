#pragma once

#include <GLFW/glfw3.h>

typedef void (*ButtonHandle)(int button, int action);

class ControlHandler
{
public:
	ControlHandler(GLFWwindow* window);

	GLFWwindow* window = nullptr;

	bool keyboardIsPressed[1024];
	ButtonHandle keyboardHandles[1024];

	struct {
		double currentX;
		double currentY;
		double lastX;
		double lastY;
		double diffX;
		double diffY;
	} mousePosition;

	bool mouseButtonIsPressed[8];
	ButtonHandle mouseButtonHandles[1024];

	void keyboardCallback(int key, int scancode, int action, int mods);
	void mousePositionCallback(double x, double y);
	void mouseButtonCallback(int button, int action, int mods);
	void clearMousePositionDiff();
	void updateMousePosition();
private:
	void _buttonCallback(bool* isPressed, ButtonHandle* handles, int button, int action, int mods);
};
