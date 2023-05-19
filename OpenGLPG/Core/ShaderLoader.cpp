#include "OpenGLPG_Base.h"

#include "ShaderLoader.h"

// for hardcoded shader names
// @improvement: remove hardcoded names
#include "Defines.h"

void ShaderLoader::OnLoad()
{
    // @improvement: load from file
    const char* vertexShaderSource = "#version 450 core\n"
                                     "layout (location = 0) in vec3 aPos;\n"
                                     "void main()\n"
                                     "{\n"
                                     "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                     "}\0";
    const char* fragmentShaderSource = "#version 450 core\n"
                                       "out vec4 FragColor;\n"
                                       "void main()\n"
                                       "{\n"
                                       "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                       "}\n";

    myShaders[VERTEX_SHADER_DEFAULT_NAME] = vertexShaderSource;
    myShaders[FRAGMENT_SHADER_DEFAULT_NAME] = fragmentShaderSource;
}

const std::string& ShaderLoader::GetShader(const std::string& aFilePath)
{
    return myShaders.at(aFilePath);
}
