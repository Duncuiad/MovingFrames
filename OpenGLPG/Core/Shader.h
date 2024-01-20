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

    Shader(const Filepath& aVertexPath, const Filepath& aFragmentPath, const Filepath* aGeometryPath = nullptr);
    ~Shader();

    void Use() const;
    bool GetUniformInt(const char* aName, int& aValueOut) const;
    bool GetUniformFloat(const char* aName, float& aValueOut) const;
    void SetUniformInt(const char* aName, int aValue) const;
    void SetUniformFloat(const char* aName, float aValue) const;
    void SetUniformMat3(const char* aName, const glm::mat3& aValue) const;
    void SetUniformMat4(const char* aName, const glm::mat4& aValue) const;

private:
    std::string LoadShaderSource(const Filepath& aPath) const;
    void LinkShaderSource(const char* aVertexSource, const char* aFragmentSource,
                          const char* aGeometrySource = nullptr);
    GLUID myID {0};
};
