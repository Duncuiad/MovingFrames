#pragma once

#include "ClientLoader.h"
#include "Game.h"

#include <memory>

struct GLFWwindow;

class Client
{
public:
    Client();

    bool IsRunning() const;

    void Init();
    void Shutdown();

    void Update();

private:
    bool GLFWInit();
    bool GLADInit();
    void ImGuiInit();
    void ImGuiShutdown();
    void ImGuiFrameStart();
    void ImGuiFrameEnd();
    void ImGuiDebugDrawClient(float anUpdateElapsedTime, float aFrameTime) const;

    void ProcessInput();
    void AddKeyboardInput(int aGLFWInput, const char* aGameInputName);
    void AddGamepadInput(int aGLFWInput, const char* aGameInputName);

    // @improvement: wrap and centralize window logic
    GLFWwindow* myWindow {nullptr};
    double myLastFrame {0.f};
    double myLastUpdate {0.f};
    ClientLoader myLoader;
    std::unique_ptr<Game> myGame {nullptr};
};
