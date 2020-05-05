#pragma once
#include "Camera.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class PerspectiveCamera : public Camera {
public:
	PerspectiveCamera(GLfloat fov = 45.0f, GLfloat aspect = 1.0f, GLfloat near = 0.0f, GLfloat far = 100.0f);

	glm::mat4 getProjection();

	GLfloat fov, aspect, near, far;
};
