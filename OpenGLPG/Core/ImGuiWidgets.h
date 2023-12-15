#pragma once

#include "MovingFrame.h"

#include <string>

namespace Widgets
{
bool ButtonWithInputText(const char* aButtonLabel, std::string& anInputTextOut, ImGuiInputTextFlags someFlags = 0);

bool OrientationWidget(const char* aLabel, MovingFrame::Coord aCoord, MovingFrame& aFrameInOut);
bool PositionWidget(const char* aLabel, MovingFrame::Coord aCoord, MovingFrame& aFrameInOut);
bool AngularVelocityWidget(const char* aLabel, MovingFrame::Coord aCoord, MovingFrame& aFrameInOut);
bool VelocityWidget(const char* aLabel, MovingFrame::Coord aCoord, MovingFrame& aFrameInOut);

template <typename EnumT>
bool RadioButton(const char* aLabel, EnumT* aValuePtr, EnumT aCase);
} // namespace Widgets

template <typename EnumT>
bool Widgets::RadioButton(const char* aLabel, EnumT* aValuePtr, EnumT aCase)
{
    int* v {reinterpret_cast<int*>(aValuePtr)};
    const int v_button {static_cast<int>(aCase)};
    const bool result {ImGui::RadioButton(aLabel, v, v_button)};
    return result;
}
