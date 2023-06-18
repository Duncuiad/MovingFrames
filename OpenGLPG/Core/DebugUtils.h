#pragma once

#include "DebugImGui.h"
#include "MathDefines.h"

class MovingFrame;

#if DEBUG_IMGUI
namespace ImGui
{
void Draw(const Vec3& aVector);
void Draw(const Quat& aQuat);
void Draw(const DualQuat& aDualQuat);
void Draw(const MovingFrame& aMovingFrame);
void DrawVecHeader();
void DrawQuatHeader();
void DrawDualQuatHeader();
} // namespace ImGui
#endif
