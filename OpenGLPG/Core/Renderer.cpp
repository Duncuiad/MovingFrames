#include "OpenGLPG_Base.h"

#include "Renderer.h"

#include <GLFW/glfw3.h>
#include <iostream>

Renderer::Renderer(const ConstructionParams& someParams)
    : myShaderLoader {someParams.myShaderLoader}
{
    SetClearColor();
}

void Renderer::Init(const InitParams& someParams)
{
    // @improvement: directories should probably be handled by Client or ClientLoader
    myShaderLoader.OnLoad("../Data/Assets/Shaders/");
}

void Renderer::Shutdown() {}

void Renderer::Render(const RenderParams& someParams)
{
    glClear(GL_COLOR_BUFFER_BIT);

    if (someParams.myWorldModel)
    {
        {
            float vertices[] = {
                0.5f,  0.5f,  0.0f, // top right
                0.5f,  -0.5f, 0.0f, // bottom right
                -0.5f, -0.5f, 0.0f, // bottom left
                -0.5f, 0.5f,  0.0f  // top left
            };
            unsigned int indices[] = {
                // note that we start from 0!
                0, 1, 3, // first triangle
                1, 2, 3  // second triangle
            };

            // vao and vbo could be done just once at initialization
            GLUID vao;
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);
            GLUID vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            GLUID ebo;
            glGenBuffers(1, &ebo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            Shader::Ptr shader {myShaderLoader.GetShader("basic.shader")};
            shader->Use();

            glBindVertexArray(vao);
            if (myIsWireframe)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            else
            {
                glPolygonMode(GL_FRONT, GL_FILL);
            }
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
    }
}

void Renderer::SetClearColor()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}
