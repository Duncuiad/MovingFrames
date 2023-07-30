#include "OpenGLPG_Base.h"

#include "Client.h"

#include "Assert.h"
#include "ClientCallbacks.h"
#include "DebugImGui.h"
#include "Defines.h"
#include "Game.h"
#include "GameInputData.h"
#include "LevelEditor.h"
#include "Test.h"

#include <format>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include <string>

constexpr float locMinFrameTime {1.f / 60.f};

Client::Client()
    : myLoader {{GLOBALPATH_ASSETFOLDER}}
{
    bool result = true;
    result &= GLFWInit();
    result &= GLADInit();
    ASSERT(result, "Failed initializing OpenGL");

    myGame = std::make_unique<LevelEditor>(Game::ConstructionParams {myLoader});
}

bool Client::IsRunning() const
{
    return !glfwWindowShouldClose(myWindow);
}

void Client::Init()
{
    Test::RunCustomRuntimeTests();

    ImGuiInit();
    myGame->Init();
    glfwSetTime(0.0);
}

void Client::Shutdown()
{
    myGame->Shutdown();
    ImGuiShutdown();

    glfwTerminate();
}

void Client::Update()
{
    double currentTime {glfwGetTime()};
    const float deltaTime = static_cast<float>(currentTime - myLastFrame);
    const float updateElapsedTime = static_cast<float>(currentTime - myLastUpdate);

    if (deltaTime >= locMinFrameTime)
    {
        ProcessInput();

        ImGuiFrameStart();
        myGame->Update({deltaTime});

        ImGuiDebugDrawClient(updateElapsedTime, deltaTime);

        ImGuiFrameEnd();
        glfwSwapBuffers(myWindow);
        glfwPollEvents();

        myLastFrame = currentTime;
    }

    myLastUpdate = currentTime;
}

bool Client::GLFWInit()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, OPENGL_PROFILE);
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

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

void Client::ImGuiInit()
{
#if DEBUG_IMGUI
    const std::string glslVersion {std::format("#version {}{}0", OPENGL_VERSION_MAJOR, OPENGL_VERSION_MINOR)};
    ImGui::CreateContext();
    [[maybe_unused]] bool imguiResult = true;
    imguiResult &= ImGui_ImplGlfw_InitForOpenGL(myWindow, true);
    imguiResult &= ImGui_ImplOpenGL3_Init(glslVersion.c_str());
    ASSERT(imguiResult, "Failed initializing ImGui");
#endif
}

void Client::ImGuiShutdown()
{
#if DEBUG_IMGUI
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
#endif
}

void Client::ImGuiFrameStart()
{
#if DEBUG_IMGUI
    ImGui_ImplGlfw_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();
#endif
}

void Client::ImGuiFrameEnd()
{
#if DEBUG_IMGUI
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
}

void Client::ImGuiDebugDrawClient(float anUpdateElapsedTime, float aFrameTime) const
{
#if DEBUG_IMGUI
    ImGui::Begin("Client");
    ImGui::Text("Update time: %9.4f ms", (anUpdateElapsedTime * 1000.f));
    ImGui::Text("Capped frame time: %9.4f ms", (aFrameTime * 1000.f));
    ImGui::End();
#endif
}

void Client::ProcessInput()
{
    ASSERT(myGame.get() != nullptr, "No Game instance exists");
    myGame->ResetGameInputData();

    AddKeyboardInput(GLFW_KEY_LEFT, GameInput::theKeyboardLookLeft);
    AddKeyboardInput(GLFW_KEY_RIGHT, GameInput::theKeyboardLookRight);
    AddKeyboardInput(GLFW_KEY_UP, GameInput::theKeyboardLookUp);
    AddKeyboardInput(GLFW_KEY_DOWN, GameInput::theKeyboardLookDown);

    AddKeyboardInput(GLFW_KEY_W, GameInput::theKeyboardMoveForward);
    AddKeyboardInput(GLFW_KEY_A, GameInput::theKeyboardMoveLeft);
    AddKeyboardInput(GLFW_KEY_S, GameInput::theKeyboardMoveBackward);
    AddKeyboardInput(GLFW_KEY_D, GameInput::theKeyboardMoveRight);
    AddKeyboardInput(GLFW_KEY_Q, GameInput::theKeyboardMoveDown);
    AddKeyboardInput(GLFW_KEY_E, GameInput::theKeyboardMoveUp);
}

void Client::AddKeyboardInput(int aGLFWInput, const char* aGameInputName)
{
    if (glfwGetKey(myWindow, aGLFWInput) == GLFW_PRESS)
    {
        myGame->AddGameInput(aGameInputName, 1.f);
    }
}

void Client::AddGamepadInput(int /*aGLFWInput*/, const char* /*aGameInputName*/)
{
    // @todo: implement method
}
