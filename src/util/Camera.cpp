#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 up) :
	position(position),
	direction(direction),
	up(up) {
}

void Camera::lookAt(glm::vec3 target) {
	direction = glm::normalize(target - position);
}

glm::mat4 Camera::getView() {
	return glm::lookAt(position, position + direction, up);
}

glm::vec3 Camera::getRight()
{
	return glm::normalize(glm::cross(direction, up));
}
