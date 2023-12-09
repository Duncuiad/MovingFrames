#pragma once

#include "Game.h"

#include <functional>
#include <memory>

struct GLFWwindow;

class ClientInputManager
{
public:
    ClientInputManager(std::reference_wrapper<GLFWwindow*> aWindow, std::unique_ptr<Game>& aGame);

    void ProcessInput();

private:
    void AddMouseInput();
    void AddMouseButtonInput(int aMouseButton, const char* aClickName, const char* aHoldName, const char* aReleaseName,
                             bool& anIsDownInOut);
    void AddKeyboardInput(int aGLFWInput, const char* aGameInputName);
    void AddGamepadInput(int aGLFWInput, const char* aGameInputName);

    std::reference_wrapper<GLFWwindow*> myWindow;
    std::unique_ptr<Game>& myGame;

    bool myLeftMouseButtonDown {false};
    bool myRightMouseButtonDown {false};
};
