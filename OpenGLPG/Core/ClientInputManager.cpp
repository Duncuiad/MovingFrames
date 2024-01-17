#include "OpenGLPG_Base.h"

#include "ClientInputManager.h"

#include "Assert.h"
#include "GameInputData.h"

#include <imgui.h>

ClientInputManager::ClientInputManager(std::reference_wrapper<GLFWwindow*> aWindow, std::unique_ptr<Game>& aGame)
    : myWindow {aWindow}
    , myGame {aGame}
{}

void ClientInputManager::ProcessInput()
{
    ASSERT(myGame.get() != nullptr, "No Game instance exists");
    myGame->ResetGameInputData();

    AddMouseInput();
    AddMouseButtonInput(GLFW_MOUSE_BUTTON_LEFT, GameInput::theMouseClickLeft, GameInput::theMouseHoldLeft,
                        GameInput::theMouseReleaseLeft, myLeftMouseButtonDown);
    AddMouseButtonInput(GLFW_MOUSE_BUTTON_RIGHT, GameInput::theMouseClickRight, GameInput::theMouseHoldRight,
                        GameInput::theMouseReleaseRight, myRightMouseButtonDown);

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

    AddKeyboardInput(GLFW_KEY_P, GameInput::theTakeScreenshot);

    if (glfwGetKey(myWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        ImGui::OpenPopup("##ExitGame");
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    }
}

void ClientInputManager::AddMouseInput()
{
    double xPos {-1.};
    double yPos {-1.};
    if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
    {
        glfwGetCursorPos(myWindow, &xPos, &yPos);

        int width {0};
        int height {0};
        glfwGetWindowSize(myWindow, &width, &height);
        double widthDouble {static_cast<double>(width)};
        double heightDouble {static_cast<double>(height)};

        if (xPos < 0. || xPos > widthDouble || yPos < 0. || yPos > heightDouble)
        {
            xPos = -1.;
            yPos = -1.;
        }
    }
    myGame->AddGameInput(GameInput::theMousePosX, static_cast<float>(xPos));
    myGame->AddGameInput(GameInput::theMousePosY, static_cast<float>(yPos));
}

void ClientInputManager::AddMouseButtonInput(int aMouseButton, const char* aClickName, const char* aHoldName,
                                             const char* aReleaseName, bool& anIsDownInOut)
{
    const int state {glfwGetMouseButton(myWindow, aMouseButton)};
    if (state == GLFW_PRESS)
    {
        if (anIsDownInOut)
        {
            myGame->AddGameInput(aHoldName, 1.f);
        }
        else
        {
            myGame->AddGameInput(aClickName, 1.f);
        }
        anIsDownInOut = true;
    }
    else
    {
        if (anIsDownInOut)
        {
            myGame->AddGameInput(aReleaseName, 1.f);
        }
        anIsDownInOut = false;
    }
}

void ClientInputManager::AddKeyboardInput(int aGLFWInput, const char* aGameInputName)
{
    if (glfwGetKey(myWindow, aGLFWInput) == GLFW_PRESS)
    {
        myGame->AddGameInput(aGameInputName, 1.f);
    }
}

void ClientInputManager::AddGamepadInput(int /*aGLFWInput*/, const char* /*aGameInputName*/)
{
    // @todo: implement method
}
