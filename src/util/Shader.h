#pragma once

#include <GL/glew.h>

class Shader {
public:
    GLuint program = 0;
    Shader();
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    void use();
};
