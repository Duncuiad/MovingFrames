#include "OpenGLPG_Base.h"

#include "Shader.h"

#include "Assert.h"

#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <sstream>

Shader::Shader(const Filepath& aVertexPath, const Filepath& aFragmentPath)
{
    ASSERT(aVertexPath.HasExtension("vert"), "Trying to load wrong file format. {} is not a vertex shader!",
           aVertexPath.GetBuffer());
    ASSERT(aFragmentPath.HasExtension("frag"), "Trying to load wrong file format. {} is not a fragment shader!",
           aFragmentPath.GetBuffer());
    const std::string vertexShaderSource {LoadShaderSource(aVertexPath)};
    const std::string fragmentShaderSource {LoadShaderSource(aFragmentPath)};
    LinkShaderSource(vertexShaderSource.data(), fragmentShaderSource.data());
}

Shader::~Shader()
{
    glDeleteProgram(myID);
}

void Shader::Use() const
{
    glUseProgram(myID);
}

std::string Shader::LoadShaderSource(const Filepath& aPath) const
{
    std::string shaderSource;

    std::ifstream shaderFile;
    shaderFile.open(aPath.GetBuffer());
    ASSERT(shaderFile.is_open(), "Failed opening shader {}", aPath.GetBuffer());

    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    shaderFile.close();
    shaderSource = shaderStream.str();

    ASSERT(shaderFile.good(), "Failed loading shader {}", aPath.GetBuffer());

    return shaderSource;
}

void Shader::LinkShaderSource(const char* aVertexSource, const char* aFragmentSource)
{
    int success;
    char infoLog[512];

    GLUID vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &aVertexSource, nullptr);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    GLUID fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &aFragmentSource, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    myID = glCreateProgram();
    glAttachShader(myID, vertexShader);
    glAttachShader(myID, fragmentShader);
    glLinkProgram(myID);
    glGetProgramiv(myID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(myID, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
