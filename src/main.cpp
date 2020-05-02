#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "control.h"
#include "Renderer.h"
#include "util/Log.h"

#define WINDOWED

int main() {
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
	int monitorWidth = vidmode->width, monitorHeight = vidmode->height;
#endif
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "noh", monitor, nullptr);

	if (window == nullptr) {
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

	glfwSetKeyCallback(window, keyCallback);

	Renderer renderer(screenWidth, screenHeight);

	while (glfwWindowShouldClose(window) == GLFW_FALSE) {
		glfwPollEvents();

		renderer.render(glfwGetTime());

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}