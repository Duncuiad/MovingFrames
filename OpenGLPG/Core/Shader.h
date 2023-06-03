#pragma once

#include "Defines.h"
#include "Filepath.h"

#include <glm/matrix.hpp>
#include <memory>
#include <string>

class Shader
{
public:
    using Ptr = std::shared_ptr<const Shader>;

    Shader(const Filepath& aVertexPath, const Filepath& aFragmentPath);
    ~Shader();

    void Use() const;
    void SetUniformInt(const char* aName, int aValue) const;
    void SetUniformFloat(const char* aName, float aValue) const;
    void SetUniformMat4(const char* aName, const glm::mat4& aValue) const;

private:
    std::string LoadShaderSource(const Filepath& aPath) const;
    void LinkShaderSource(const char* aVertexSource, const char* aFragmentSource);
    GLUID myID {0};
};
