#pragma once
#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class PerspectiveCamera : public Camera {
public:
	PerspectiveCamera(float fov = 45.0f, float aspect = 1.0f, float near = 0.0f, float far = 100.0f);

	glm::mat4 getProjection();

	float fov;
	float aspect;
	float near;
	float far;
};

