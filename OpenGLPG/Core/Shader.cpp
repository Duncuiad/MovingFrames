#include "OpenGLPG_Base.h"

#include "Shader.h"

#include "Assert.h"

#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <sstream>

Shader::Shader(const Filepath& aVertexPath, const Filepath& aFragmentPath, const Filepath* aGeometryPath /*= nullptr*/)
{
    ASSERT(aVertexPath.HasExtension("vert"), "Trying to load wrong file format. {} is not a vertex shader!",
           aVertexPath.GetBuffer());
    ASSERT(aFragmentPath.HasExtension("frag"), "Trying to load wrong file format. {} is not a fragment shader!",
           aFragmentPath.GetBuffer());
    ASSERT(aGeometryPath == nullptr || aGeometryPath->HasExtension("geom"),
           "Trying to load wrong file format. {} is not a geometry shader!", aGeometryPath->GetBuffer());
    const std::string vertexShaderSource {LoadShaderSource(aVertexPath)};
    const std::string fragmentShaderSource {LoadShaderSource(aFragmentPath)};

    if (aGeometryPath != nullptr)
    {
        const std::string geometryShaderSource {LoadShaderSource(*aGeometryPath)};
        LinkShaderSource(vertexShaderSource.data(), fragmentShaderSource.data(), geometryShaderSource.data());
    }
    else
    {
        LinkShaderSource(vertexShaderSource.data(), fragmentShaderSource.data());
    }
}

Shader::~Shader()
{
    glDeleteProgram(myID);
}

void Shader::Use() const
{
    glUseProgram(myID);
}

bool Shader::GetUniformInt(const char* aName, int& aValueOut) const
{
    const int location {glGetUniformLocation(myID, aName)};
    if (location != -1)
    {
        glGetUniformiv(myID, location, &aValueOut);
        return true;
    }
    return false;
}

bool Shader::GetUniformFloat(const char* aName, float& aValueOut) const
{
    const int location {glGetUniformLocation(myID, aName)};
    if (location != -1)
    {
        glGetUniformfv(myID, location, &aValueOut);
        return true;
    }
    return false;
}

void Shader::SetUniformInt(const char* aName, int aValue) const
{
    glUniform1i(glGetUniformLocation(myID, aName), aValue);
}

void Shader::SetUniformFloat(const char* aName, float aValue) const
{
    glUniform1f(glGetUniformLocation(myID, aName), aValue);
}

void Shader::SetUniformMat3(const char* aName, const glm::mat3& aValue) const
{
    glUniformMatrix3fv(glGetUniformLocation(myID, aName), 1, false, &aValue[0][0]);
}

void Shader::SetUniformMat4(const char* aName, const glm::mat4& aValue) const
{
    glUniformMatrix4fv(glGetUniformLocation(myID, aName), 1, false, &aValue[0][0]);
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

void Shader::LinkShaderSource(const char* aVertexSource, const char* aFragmentSource,
                              const char* aGeometrySource /*= nullptr*/)
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

    GLUID geometryShader {0u};
    if (aGeometrySource != nullptr)
    {
        geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometryShader, 1, &aGeometrySource, nullptr);
        glCompileShader(geometryShader);
        glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(geometryShader, 512, nullptr, infoLog);
            std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }

    myID = glCreateProgram();
    glAttachShader(myID, vertexShader);
    if (geometryShader != 0u)
    {
        glAttachShader(myID, geometryShader);
    }
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
    if (geometryShader != 0u)
    {
        glDeleteShader(geometryShader);
    }
}
