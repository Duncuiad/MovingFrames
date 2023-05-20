#pragma once

#include "Defines.h"
#include "Filepath.h"

#include <memory>
#include <string>

class Shader
{
public:
    using Ptr = std::shared_ptr<const Shader>;

    Shader(const Filepath& aVertexPath, const Filepath& aFragmentPath);

    void Use() const;

private:
    std::string LoadShaderSource(const Filepath& aPath) const;
    void LinkShaderSource(const char* aVertexSource, const char* aFragmentSource);
    GLUID myID;
};
