#include "OpenGLPG_Base.h"

#include "Client.h"

#include "ClientCallbacks.h"
#include "Defines.h"
#include "Game.h"

#include <cassert>
#include <iostream>

Client::Client()
{
    bool result = true;
    result &= GLFWInit();
    result &= GLADInit();
    assert(result);

    myGame = std::make_unique<Game>(Game::ConstructionParams {myLoader});
}

bool Client::IsRunning() const
{
    return !glfwWindowShouldClose(myWindow);
}

void Client::Init()
{
    myGame->Init();
}

void Client::Shutdown()
{
    myGame->Shutdown();

    glfwTerminate();
}

void Client::Update()
{
    ProcessInput();

    myGame->Update();

    glfwSwapBuffers(myWindow);
    glfwPollEvents();
}

bool Client::GLFWInit()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, OPENGL_PROFILE);

    myWindow = glfwCreateWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, "OpenGLPG", nullptr, nullptr);
    if (myWindow == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(myWindow);

    // Register input callbacks here
    glfwSetFramebufferSizeCallback(myWindow, ClientCB::FrameBufferSizeCallback);
    return true;
}

bool Client::GLADInit()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    return true;
}

void Client::ProcessInput()
{
}
