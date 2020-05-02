#include "PerspectiveCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float near, float far) :
	fov(fov), aspect(aspect), near(near), far(far) {
}

glm::mat4 PerspectiveCamera::getProjection() {
	return glm::perspective(glm::radians(fov), aspect, near, far);
}
