#include "OpenGLPG_Base.h"

#include "ImGuiWidgets.h"

namespace Widgets
{
bool ButtonWithInputText(const char* aButtonLabel, std::string& anInputTextOut, ImGuiInputTextFlags someFlags)
{
    constexpr int inputTextLength {512};
    char inputText[inputTextLength];
    strcpy_s(inputText, anInputTextOut.data());

    std::string label {aButtonLabel};
    label = "##" + label + "_Input";
    if (ImGui::InputText(label.data(), inputText, inputTextLength, someFlags))
    {
        anInputTextOut = inputText;
    }
    ImGui::SameLine();
    return ImGui::Button(aButtonLabel) && strlen(inputText) != 0;
}

bool OrientationWidget(const char* aLabel, MovingFrame& aFrameInOut)
{
    Quat quat {aFrameInOut.GetOrientation()};
    if (ImGui::InputFloat4(aLabel, &quat[0], "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
    {
        aFrameInOut.SetOrientation(glm::normalize(quat));
        return true;
    }
    return false;
}

bool PositionWidget(const char* aLabel, MovingFrame& aFrameInOut)
{
    Vec3 vec {aFrameInOut.GetPosition()};
    if (ImGui::InputFloat3(aLabel, &vec[0], "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
    {
        aFrameInOut.SetPosition(vec);
        return true;
    }
    return false;
}

bool AngularVelocityWidget(const char* aLabel, MovingFrame& aFrameInOut)
{
    constexpr MovingFrame::Coord coord {MovingFrame::Coord::Intrinsic};
    Vec3 vec {aFrameInOut.GetAngularVelocity(coord)};
    if (ImGui::InputFloat3(aLabel, &vec[0], "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
    {
        aFrameInOut.SetAngularVelocity(vec, coord);
        return true;
    }
    return false;
}

bool VelocityWidget(const char* aLabel, MovingFrame& aFrameInOut)
{
    constexpr MovingFrame::Coord coord {MovingFrame::Coord::Intrinsic};
    Vec3 vec {aFrameInOut.GetLinearVelocity(coord)};
    if (ImGui::InputFloat3(aLabel, &vec[0], "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
    {
        aFrameInOut.SetLinearVelocity(vec, coord);
        return true;
    }
    return false;
}
} // namespace Widgets
