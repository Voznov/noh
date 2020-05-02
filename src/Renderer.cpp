#include "Renderer.h"

#include <GL/glew.h>
#include <SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

	camera = PerspectiveCamera(45.0f, (float)screenWidth / screenHeight, 0.1f, 100.0f);
	camera.position = glm::vec3(0.0f, 0.0f, 10.0f);
	camera.up = glm::vec3(0.0f, 1.0f, 0.0f);
	camera.lookAt(glm::vec3(0.0f, 0.0f, 0.0f));

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 5 * verticesSize * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Выравнивание в 1 пиксель, для картинок с шириной не кратной 4.
	int texWidth, texHeight, texChannels;
	unsigned char* texData = SOIL_load_image(texFilename, &texWidth, &texHeight, &texChannels, SOIL_LOAD_RGB);
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
	SOIL_free_image_data(texData);
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
	glUniformMatrix4fv(modelView, 1, GL_FALSE, glm::value_ptr(camera.getView()));
	GLint modelProjection = glGetUniformLocation(shader.program, "projection");
	glUniformMatrix4fv(modelProjection, 1, GL_FALSE, glm::value_ptr(camera.getProjection()));


	shader.use();
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 3 * indicesSize, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

Renderer::~Renderer()
{
	delete[] vertices;
	delete[] indices;
}
