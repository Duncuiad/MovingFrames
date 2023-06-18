#include "OpenGLPG_Base.h"

#include "DebugUtils.h"

#include "MovingFrame.h"

#if DEBUG_IMGUI
namespace ImGui
{
void Draw(const Vec3& aVector)
{
    ImGui::Text("%9.4f, %9.4f, %9.4f", aVector.x, aVector.y, aVector.z);
}
void Draw(const Quat& aQuat)
{
    ImGui::Text("%9.4f, %9.4f, %9.4f, %9.4f", aQuat.x, aQuat.y, aQuat.z, aQuat.w);
}
void Draw(const DualQuat& aDualQuat)
{
    ImGui::Text("PR:  ");
    ImGui::SameLine();
    ImGui::Draw(aDualQuat.real);

    ImGui::Text("DU:  ");
    ImGui::SameLine();
    ImGui::Draw(aDualQuat.dual);

    ImGui::Text("Pos: ");
    ImGui::SameLine();
    ImGui::Draw(aDualQuat.dual * glm::conjugate(aDualQuat.real) * 2.f);
}
void Draw(const MovingFrame& aMovingFrame)
{
    ImGui::Text("Pose:");
    ImGui::Indent();
    ImGui::Draw(aMovingFrame.myPose);
    ImGui::Unindent();
    ImGui::Text("Twist:");
    ImGui::Indent();
    ImGui::Draw(aMovingFrame.myTwist);
    ImGui::Unindent();
}
void DrawVecHeader()
{
    ImGui::Text("      X          Y          Z");
}
void DrawQuatHeader()
{
    ImGui::Text("      X          Y          Z          W");
}
void DrawDualQuatHeader()
{
    ImGui::Indent();
    DrawQuatHeader();
    ImGui::Unindent();
}
} // namespace ImGui
#endif
