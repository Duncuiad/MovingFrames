#include "OpenGLPG_Base.h"

#include "FrameSplineEditorWidget.h"

#include "FrameSpline.h"
#include "ImGuiWidgets.h"

void FrameSplineEditorWidget::Draw()
{
    bool changed {false};

    auto& keys {GetKeyFrames()};
    ImGui::PushID(("FrameSpline" + myUID.GetString()).data());

    changed |= DrawNewKeyAtBeginning();

    for (int i = 0; i < keys.Count(); ++i)
    {
        changed |= DrawKeyFrame(i);
    }

    changed |= DrawNewKeyAtEnd();
    changed |= DrawDeletePopup();

    ImGui::PopID();

    if (changed)
    {
        myOnChanged();
    }
}

bool FrameSplineEditorWidget::DrawNewKeyAtBeginning()
{
    bool changed {false};

    if (GetKeyFrames().Count())
    {
        ImGui::PushID("_Beginning");
        if (ImGui::Button("New Key"))
        {
            AddKeyFrameAtBeginning();
            changed = true;
        }
        ImGui::PopID();
    }

    return changed;
}

bool FrameSplineEditorWidget::DrawKeyFrame(int anIndex)
{
    bool changed {false};

    ImGui::PushID(anIndex);

    changed |= DrawKeySeparator(anIndex);
    changed |= DrawKeyHeader(anIndex);

    MovingFrame& key {GetKeyFrames()[anIndex].myFrame};
    changed |= Widgets::OrientationWidget("Ori", key);
    changed |= Widgets::PositionWidget("Pos", key);
    changed |= Widgets::VelocityWidget("Vel", key);
    changed |= Widgets::AngularVelocityWidget("Ang", key);

    ImGui::PopID();

    return changed;
}

bool FrameSplineEditorWidget::DrawKeySeparator(int anIndex)
{
    if (anIndex == 0)
    {
        return false;
    }

    bool changed {false};
    auto& keys {GetKeyFrames()};

    if (ImGui::Button("X"))
    {
        SwapKeyFramesAtIndex(anIndex);
        changed = true;
    }

    const bool hasJump {keys[anIndex - 1].myTiming >= keys[anIndex].myTiming};
    if (!hasJump)
    {
        ImGui::SameLine();
        if (ImGui::Button("+"))
        {
            InsertKeyFrameAtIndex(anIndex);
            changed = true;
        }
    }

    return changed;
}

bool FrameSplineEditorWidget::DrawKeyHeader(int anIndex)
{
    bool changed {false};
    auto& keys {GetKeyFrames()};

    const float prevT {anIndex == 0 ? 0.f : glm::max(0.f, keys[anIndex - 1].myTiming)};
    const float nextT {anIndex + 1 >= keys.Count() ? 1.f : glm::min(1.f, keys[anIndex + 1].myTiming)};
    float currT {keys[anIndex].myTiming};
    if (ImGui::SliderFloat("##T", &currT, 0.f, 1.f, "T = %.3f", ImGuiSliderFlags_AlwaysClamp))
    {
        const float oldT {keys[anIndex].myTiming};
        keys[anIndex].myTiming = glm::clamp(currT, prevT, nextT);
        changed |= oldT != keys[anIndex].myTiming;
    }
    ImGui::SameLine();
    if (ImGui::Button("Delete"))
    {
        myKeyToDelete = anIndex;
    }

    return changed;
}

bool FrameSplineEditorWidget::DrawDeletePopup()
{
    if (myKeyToDelete == -1)
    {
        return false;
    }

    bool changed {false};

    ImGui::OpenPopup("##DeleteKeyFrame");
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("##DeleteKeyFrame"))
    {
        auto& keys {GetKeyFrames()};
        ImGui::Text("Deleting key at T = %.3f", keys[myKeyToDelete].myTiming);
        if (ImGui::Button("Confirm"))
        {
            keys.RemoveAt(myKeyToDelete);
            changed = true;

            myKeyToDelete = -1;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            myKeyToDelete = -1;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    return changed;
}

bool FrameSplineEditorWidget::DrawNewKeyAtEnd()
{
    bool changed {false};
    if (ImGui::Button("New Key"))
    {
        AddKeyFrameAtEnd();
        changed = true;
    }
    return changed;
}
