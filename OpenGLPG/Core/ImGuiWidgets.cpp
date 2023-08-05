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
    if (ImGui::DragFloat4(aLabel, &quat[0], 0.01f, -1.f, 1.f, "%.3f"))
    {
        aFrameInOut.SetOrientation(glm::normalize(quat));
        return true;
    }
    return false;
}

bool PositionWidget(const char* aLabel, MovingFrame& aFrameInOut)
{
    Vec3 vec {aFrameInOut.GetPosition()};
    if (ImGui::DragFloat3(aLabel, &vec[0], 0.1f, 0.f, 0.f, "%.3f"))
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
    if (ImGui::DragFloat3(aLabel, &vec[0], 0.01f, 0.f, 0.f, "%.3f"))
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
    if (ImGui::DragFloat3(aLabel, &vec[0], 0.1f, 0.f, 0.f, "%.3f"))
    {
        aFrameInOut.SetLinearVelocity(vec, coord);
        return true;
    }
    return false;
}
} // namespace Widgets
