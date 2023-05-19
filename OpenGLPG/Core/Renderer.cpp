#include "OpenGLPG_Base.h"

#include "Renderer.h"

#include <GLFW/glfw3.h>
#include <iostream>

Renderer::Renderer(const ConstructionParams& someParams) : myShaderLoader {someParams.myShaderLoader}
{
    SetClearColor();
}

void Renderer::Init(const InitParams& someParams)
{
    myShaderLoader.OnLoad();

    const char* vertexShaderSource {myShaderLoader.GetShader(VERTEX_SHADER_DEFAULT_NAME).data()};
    const char* fragmentShaderSource {myShaderLoader.GetShader(FRAGMENT_SHADER_DEFAULT_NAME).data()};

    LinkShaderProgram(vertexShaderSource, fragmentShaderSource);
}

void Renderer::LinkShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource)
{
    int success;
    char infoLog[512];

    GLUID vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    GLUID fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    myShaderProgram = glCreateProgram();
    glAttachShader(myShaderProgram, vertexShader);
    glAttachShader(myShaderProgram, fragmentShader);
    glLinkProgram(myShaderProgram);
    glGetProgramiv(myShaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(myShaderProgram, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Renderer::Shutdown()
{
}

void Renderer::Render(const RenderParams& someParams)
{
    glClear(GL_COLOR_BUFFER_BIT);

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

        // vao and vbo could be done just one at initialization
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

        glUseProgram(myShaderProgram);
        glBindVertexArray(vao);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}

void Renderer::SetClearColor()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}
