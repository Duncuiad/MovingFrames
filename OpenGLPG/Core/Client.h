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

    void ProcessInput();

    // @improvement: wrap and centralize window logic
    GLFWwindow* myWindow {nullptr};
    ClientLoader myLoader;
    std::unique_ptr<Game> myGame {nullptr};
};
