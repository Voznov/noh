#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:
	Camera(
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 direction = glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)
	);
	
	void lookAt(glm::vec3 target);

	virtual glm::mat4 getProjection() = 0;
	glm::mat4 getView();

	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 up;
};