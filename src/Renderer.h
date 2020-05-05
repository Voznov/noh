#pragma once

#include <cstddef>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "util/Shader.h"
#include "util/PerspectiveCamera.h"

class Renderer
{
public:
	Renderer(int screenWidth, int screenHeight);
	~Renderer();

	PerspectiveCamera *camera;
	
	void render(double time);

private:
	int screenWidth, screenHeight;

	GLfloat *vertices;
	GLuint *indices;
	std::size_t verticesSize, indicesSize;

	const char *texFilename;

	const char *vertexShaderFilename, *fragmentShaderFilename;
	Shader shader;

	glm::mat4 trans = glm::mat4(1.0f);

	GLuint vao;
};
