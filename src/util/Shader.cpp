#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>

#include <GL/glew.h>

#include "Log.h"

Shader::Shader() {
}

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
    std::ifstream vertexFile(vertexPath);
    std::ifstream fragmentFile(fragmentPath);

    if (!vertexFile.is_open() && !fragmentFile.is_open()) {
        Log::err("Can't open shader file");
        return;
    }
    
    std::stringstream vShaderStream, fShaderStream;
    vShaderStream << vertexFile.rdbuf();
    fShaderStream << fragmentFile.rdbuf();

    vertexFile.close();
    fragmentFile.close();

    std::string vStringCode, fStringCode;
    vStringCode = vShaderStream.str();
    fStringCode = fShaderStream.str();

    const GLchar* vShaderCode = vStringCode.c_str();
    const GLchar* fShaderCode = fStringCode.c_str();

    GLint success;
    GLchar infoLog[512];

    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCode, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        Log::err("Can't compile vertex shader");
        Log::err(infoLog);
    };

    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        Log::err("Can't compile fragment shader");
        Log::err(infoLog);
    };

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        Log::err("Can't link shader program");
        Log::err(infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use() {
    glUseProgram(program);
}
