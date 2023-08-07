#include "OpenGLPG_Base.h"

#include "ImGuiWidgets.h"

#include <glm/ext/quaternion_exponential.hpp>

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

bool OrientationWidget(const char* aLabel, MovingFrame::Coord aCoord, MovingFrame& aFrameInOut)
{
    Quat quat {aFrameInOut.GetOrientation()};
    if (ImGui::DragFloat4(aLabel, &quat[0], 0.01f, 0.f, 0.f, "%.3f"))
    {
        const Vec3 position {aFrameInOut.GetPosition()};
        const Vec3 linearVelocity {aFrameInOut.GetLinearVelocity(MovingFrame::Extrinsic)};
        const Vec3 angularVelocity {aFrameInOut.GetAngularVelocity(MovingFrame::Extrinsic)};

        const Quat difference {quat - aFrameInOut.GetOrientation()};
        const Quat offset {glm::exp(difference)};
        if (aCoord == MovingFrame::Extrinsic)
        {
            aFrameInOut.SetOrientation(glm::normalize(offset * aFrameInOut.GetOrientation()));
        }
        else
        {
            aFrameInOut.SetOrientation(glm::normalize(aFrameInOut.GetOrientation() * offset));
        }

        aFrameInOut.SetAngularVelocity(angularVelocity, MovingFrame::Extrinsic);
        aFrameInOut.SetLinearVelocity(linearVelocity, MovingFrame::Extrinsic);
        aFrameInOut.SetPosition(position);
        return true;
    }
    return false;
}

bool PositionWidget(const char* aLabel, MovingFrame::Coord aCoord, MovingFrame& aFrameInOut)
{
    Vec3 vec {aFrameInOut.GetPosition()};
    if (ImGui::DragFloat3(aLabel, &vec[0], 0.1f, 0.f, 0.f, "%.3f"))
    {
        Vec3 offset {vec - aFrameInOut.GetPosition()};
        if (aCoord == MovingFrame::Intrinsic)
        {
            offset = aFrameInOut.GetOrientation() * offset;
        }
        aFrameInOut.SetPosition(aFrameInOut.GetPosition() + offset);
        return true;
    }
    return false;
}

bool AngularVelocityWidget(const char* aLabel, MovingFrame::Coord aCoord, MovingFrame& aFrameInOut)
{
    Vec3 vec {aFrameInOut.GetAngularVelocity(aCoord)};
    if (ImGui::DragFloat3(aLabel, &vec[0], 0.01f, 0.f, 0.f, "%.3f"))
    {
        aFrameInOut.SetAngularVelocity(vec, aCoord);
        return true;
    }
    return false;
}

bool VelocityWidget(const char* aLabel, MovingFrame::Coord aCoord, MovingFrame& aFrameInOut)
{
    Vec3 vec {aFrameInOut.GetLinearVelocity(aCoord)};
    if (ImGui::DragFloat3(aLabel, &vec[0], 0.1f, 0.f, 0.f, "%.3f"))
    {
        aFrameInOut.SetLinearVelocity(vec, aCoord);
        return true;
    }
    return false;
}
} // namespace Widgets
