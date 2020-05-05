#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define _USE_MATH_DEFINES
#include <glm/glm.hpp>

#include "Renderer.h"
#include "util/ControlHandler.h"
#include "util/Camera.h"
#include "util/Log.h"

#define WINDOWED

GLFWwindow* window;
ControlHandler* controlHandler;

GLfloat yaw, pitch;
const GLfloat minPitch = -89.9f, maxPitch = 89.9f;

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	controlHandler->keyboardCallback(key, scancode, action, mods);
}

void closeWindow(int key, int action) {
	glfwSetWindowShouldClose(window, GL_TRUE);
}

void gameControl(Camera* camera, double deltaTime) {
	const GLfloat cameraSpeed = 10.0f;
	GLfloat cameraSpeedPerFrame = cameraSpeed * deltaTime;
	glm::vec3 cameraDiff(0.0f);
	if (controlHandler->keyboardIsPressed[GLFW_KEY_W])
		cameraDiff += camera->direction;
	if (controlHandler->keyboardIsPressed[GLFW_KEY_S])
		cameraDiff -= camera->direction;
	if (controlHandler->keyboardIsPressed[GLFW_KEY_A])
		cameraDiff -= camera->getRight();
	if (controlHandler->keyboardIsPressed[GLFW_KEY_D])
		cameraDiff += camera->getRight();
	camera->position += cameraDiff * cameraSpeedPerFrame;

	const GLfloat mouseSensitivity = 0.1;
	yaw += controlHandler->mousePosition.diffX * mouseSensitivity;
	pitch -= controlHandler->mousePosition.diffY * mouseSensitivity;
	controlHandler->clearMousePositionDiff();

	if (pitch > maxPitch)
		pitch = maxPitch;
	if (pitch < minPitch)
		pitch = minPitch;

	glm::vec3 front(0.0f);
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	camera->direction = glm::normalize(front);
}

int main()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	if (glfwInit() == GLFW_FALSE)
		return -1;

#ifdef WINDOWED
	GLFWmonitor* monitor = nullptr;
	int screenWidth = 800, screenHeight = 600;
#else
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);
	int screenWidth = vidmode->width, screenHeight = vidmode->height;
#endif
	window = glfwCreateWindow(screenWidth, screenHeight, "noh", monitor, nullptr);

	if (window == nullptr)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		return -1;

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);

	glEnable(GL_DEPTH_TEST);

	// cursor
	unsigned char pixels[16 * 16 * 4];
	memset(pixels, 0xff, sizeof(pixels));

	GLFWimage image;
	image.width = 16;
	image.height = 16;
	image.pixels = pixels;

	GLFWcursor* cursor = glfwCreateCursor(&image, 0, 0);
	///cursor

	controlHandler = new ControlHandler(window);
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {controlHandler->keyboardCallback(key, scancode, action, mods); });
	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y) {controlHandler->mousePositionCallback(x, y); });
	glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {controlHandler->mouseButtonCallback(button, action, mods); });
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	controlHandler->keyboardHandles[GLFW_KEY_ESCAPE] = closeWindow;
	controlHandler->mouseButtonHandles[GLFW_MOUSE_BUTTON_1] = closeWindow;

	Renderer renderer(screenWidth, screenHeight);

	// game logic
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	yaw = -90.0f;
	pitch = 0;
	///game logic

	double time = glfwGetTime();
	double deltaTime;
	while (glfwWindowShouldClose(window) == GLFW_FALSE)
	{
		deltaTime = glfwGetTime() - time;
		time += deltaTime;

		glfwPollEvents();
		gameControl(renderer.camera, deltaTime);

		renderer.render(time);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}