#include "OpenGLPG_Base.h"

#include "LevelEditorWidget.h"

#include "EditorCmp.h"
#include "ImGuiWidgets.h"
#include "WorldModel.h"

LevelEditorWidget::LevelEditorWidget(WorldModel* aWorldModel)
    : myWorldModel {aWorldModel}
{}

void LevelEditorWidget::Draw()
{
    ImGui::Begin("Level Editor");

    DrawSaveWorld();
    DrawEntities();

    ImGui::End();
}

void LevelEditorWidget::DrawSaveWorld()
{
    if (Widgets::ButtonWithInputText("Save World", mySaveWorldName,
                                     ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank))
    {
        std::remove(mySaveWorldName.begin(), mySaveWorldName.end(), '.');
        mySaveWorldName += ".world";
        myWorldModel->SaveWorld(mySaveWorldName);
        myShowSaveWorldConfirmation = true;
        ImGui::OpenPopup("##SaveWorldConfirmation");
    }

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("##SaveWorldConfirmation", &myShowSaveWorldConfirmation))
    {
        ImGui::Text("Saved world as %s", mySaveWorldName.data());
        if (ImGui::Button("OK"))
        {
            mySaveWorldName = "";
            myShowSaveWorldConfirmation = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void LevelEditorWidget::DrawEntities()
{
    ImGui::Text("Entities:");
    for (const auto& [uid, entity] : myWorldModel->GetEntities())
    {
        const std::string entityUID {uid.GetString()};
        if (ImGui::TreeNode(("LevelEditorEntities_" + entityUID).data(), entityUID.data()))
        {
            for (EditorComponent* cmp : entity.GetEditableComponents<EditorComponent>())
            {
                // @todo: Here I'm using the Serializable class ID as a shorthand for the class name. It would be a lot
                // less smelly to create a dedicated one
                const std::string cmpName {cmp->GetSubtypeId()};
                if (ImGui::Button("Edit"))
                {
                    cmp->myIsDisplayingWidget = true;

                    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
                    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
                }
                ImGui::SameLine();
                ImGui::Text(cmpName.data());

                if (cmp->myIsDisplayingWidget)
                {
                    ImGui::Begin((cmpName + " Editor").data(), &cmp->myIsDisplayingWidget);
                    cmp->DrawWidget();
                    ImGui::End();
                }
            }
            ImGui::TreePop();
        }
    }
}
