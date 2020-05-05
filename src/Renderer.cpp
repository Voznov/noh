#include "Renderer.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "util/Shader.h"
#include "util/Log.h"

Renderer::Renderer(int screenWidth, int screenHeight) : screenWidth(screenWidth), screenHeight(screenHeight) {
	vertices = new GLfloat[120]{
	 1.0f,  1.0f, 0.0f, 1.0f, 0.0f,
	 1.0f,  0.0f, 0.0f, 1.0f, 1.0f,
	 0.0f,  0.0f, 0.0f, 0.0f, 1.0f,
	 0.0f,  1.0f, 0.0f, 0.0f, 0.0f,

	 1.0f,  1.0f, 1.0f, 1.0f, 0.0f,
	 1.0f,  0.0f, 1.0f, 1.0f, 1.0f,
	 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,
	 0.0f,  1.0f, 1.0f, 0.0f, 0.0f,

	 1.0f,  0.0f, 1.0f, 1.0f, 0.0f,
	 1.0f,  0.0f, 0.0f, 1.0f, 1.0f,
	 0.0f,  0.0f, 0.0f, 0.0f, 1.0f,
	 0.0f,  0.0f, 1.0f, 0.0f, 0.0f,

	 1.0f,  1.0f, 1.0f, 1.0f, 0.0f,
	 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
	 0.0f,  1.0f, 0.0f, 0.0f, 1.0f,
	 0.0f,  1.0f, 1.0f, 0.0f, 0.0f,

	 0.0f, 1.0f,  1.0f, 1.0f, 0.0f,
	 0.0f, 1.0f,  0.0f, 1.0f, 1.0f,
	 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
	 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,

	 1.0f, 1.0f,  1.0f, 1.0f, 0.0f,
	 1.0f, 1.0f,  0.0f, 1.0f, 1.0f,
	 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
	 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,

	};
	verticesSize = 24;
	indices = new GLuint[36]{
		0, 1, 3,
		1, 2, 3,

		4, 5, 7,
		5, 6, 7,

		8, 9, 11,
		9, 10, 11,

		12, 13, 15,
		13, 14, 15,

		16, 17, 19,
		17, 18, 19,

		20, 21, 23,
		21, 22, 23,
	};
	indicesSize = 12;

	texFilename = "res/tex/texture.bmp";

	vertexShaderFilename = "res/shader/shader.vert";
	fragmentShaderFilename = "res/shader/shader.frag";
	shader = Shader(vertexShaderFilename, fragmentShaderFilename);

	camera = new PerspectiveCamera(45.0f, (float)screenWidth / screenHeight, 0.1f, 100.0f);
	camera->position = glm::vec3(0.0f, 0.0f, 10.0f);
	camera->up = glm::vec3(0.0f, 1.0f, 0.0f);
	camera->lookAt(glm::vec3(0.0f, 0.0f, 0.0f));

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 5 * verticesSize * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	int texWidth, texHeight, texChannels;
	unsigned char* texData = stbi_load(texFilename, &texWidth, &texHeight, &texChannels, 3);
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
	stbi_image_free(texData);
	glGenerateMipmap(GL_TEXTURE_2D);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * indicesSize * sizeof(GLuint), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Renderer::render(double time) {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 model(1.0f);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	GLint modelLoc = glGetUniformLocation(shader.program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	GLint modelView = glGetUniformLocation(shader.program, "view");
	glUniformMatrix4fv(modelView, 1, GL_FALSE, glm::value_ptr(camera->getView()));
	GLint modelProjection = glGetUniformLocation(shader.program, "projection");
	glUniformMatrix4fv(modelProjection, 1, GL_FALSE, glm::value_ptr(camera->getProjection()));


	shader.use();
	glBindVertexArray(vao);
	// many blocks
	glm::vec3 cubePositions[] = {
		glm::vec3( 0.0f,  0.0f,  0.0f), 
		glm::vec3( 2.0f,  5.0f, -15.0f), 
		glm::vec3(-1.5f, -2.2f, -2.5f),  
		glm::vec3(-3.8f, -2.0f, -12.3f),  
		glm::vec3( 2.4f, -0.4f, -3.5f),  
		glm::vec3(-1.7f,  3.0f, -7.5f),  
		glm::vec3( 1.3f, -2.0f, -2.5f),  
		glm::vec3( 1.5f,  2.0f, -2.5f), 
		glm::vec3( 1.5f,  0.2f, -1.5f), 
		glm::vec3(-1.3f,  1.0f, -1.5f)  
	};
	for (GLuint i = 0; i < 10; i++)
	{
		glm::mat4 model(1.0f);
		model = glm::translate(model, cubePositions[i]);
		GLfloat angle = 20.0f * i;
		model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glDrawElements(GL_TRIANGLES, 3 * indicesSize, GL_UNSIGNED_INT, 0);
	}
	///many blocks

	// one block
	//glDrawElements(GL_TRIANGLES, 3 * indicesSize, GL_UNSIGNED_INT, 0);
	///one block
	glBindVertexArray(0);
}

Renderer::~Renderer()
{
	delete camera;
	delete[] vertices;
	delete[] indices;
}
