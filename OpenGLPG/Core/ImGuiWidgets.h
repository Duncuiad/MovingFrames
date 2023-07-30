#pragma once

#include "MovingFrame.h"

#include <string>

namespace Widgets
{
bool ButtonWithInputText(const char* aButtonLabel, std::string& anInputTextOut, ImGuiInputTextFlags someFlags = 0);

bool OrientationWidget(const char* aLabel, MovingFrame& aFrameInOut);
bool PositionWidget(const char* aLabel, MovingFrame& aFrameInOut);
bool AngularVelocityWidget(const char* aLabel, MovingFrame& aFrameInOut);
bool VelocityWidget(const char* aLabel, MovingFrame& aFrameInOut);
} // namespace Widgets
