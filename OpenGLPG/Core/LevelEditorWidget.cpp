#include "OpenGLPG_Base.h"

#include "LevelEditorWidget.h"

#include "Assert.h"
#include "EditorCmp.h"
#include "ImGuiWidgets.h"
#include "Serializer.h"
#include "World.h"

LevelEditorWidget::LevelEditorWidget(World* aWorld, const Filepath& anEntityTemplateFolder,
                                     const Filepath& anEntityTemplateIndex)
    : myWorld {aWorld}
    , myEntityTemplateFolder {anEntityTemplateFolder}
{
    ASSERT(anEntityTemplateIndex.HasExtension("templateindex"), "Wrong file extension");
    SerializerLoader loader {myEntityTemplateFolder + anEntityTemplateIndex};
    myEntityTemplates.Serialize(loader);
    for (const Filepath& entityTemplate : myEntityTemplates)
    {
        ASSERT(entityTemplate.HasExtension("template"), "Wrong file extension");
        SerializerLoader checkSerializer {myEntityTemplateFolder + entityTemplate};
    }
}

void LevelEditorWidget::Draw()
{
    ImGui::Begin("Level Editor");

    DrawSaveWorld();
    DrawAddEntity();
    DrawEntities();
    DrawDeleteEntityPopup();

    ImGui::End();
}

void LevelEditorWidget::DrawSaveWorld()
{
    if (Widgets::ButtonWithInputText("Save World", mySaveWorldName,
                                     ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank))
    {
        std::remove(mySaveWorldName.begin(), mySaveWorldName.end(), '.');
        mySaveWorldName += ".world";
        myWorld->SaveWorld(mySaveWorldName);
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

void LevelEditorWidget::DrawAddEntity()
{
    if (ImGui::Button("Add Entity"))
    {
        ImGui::OpenPopup("##AddEntity");
    }

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("##AddEntity"))
    {
        Filepath selectedTemplate {};
        ImGui::Text("Choose entity template");
        for (const Filepath& entityTemplate : myEntityTemplates)
        {
            if (ImGui::Button(entityTemplate.GetBuffer(), ImVec2(300.f, 20.f)))
            {
                selectedTemplate = entityTemplate;
            }
        }
        if (selectedTemplate != Filepath {})
        {
            myWorld->RequestEntitySpawn(myEntityTemplateFolder + selectedTemplate);
            ImGui::CloseCurrentPopup();
        }

        ImGui::Separator();
        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void LevelEditorWidget::DrawEntities()
{
    ImGui::Text("Entities:");
    for (const auto& [uid, entity] : myWorld->GetEntities())
    {
        const std::string entityUID {uid.GetString()};
        if (ImGui::TreeNode(("LevelEditorEntities_" + entityUID).data(), entityUID.data()))
        {
            DrawDeleteEntityButton(uid);

            for (EditorComponent* cmp : entity.GetEditableComponents<EditorComponent>())
            {
                // @todo: Here I'm using the Serializable class ID as a shorthand for the class name. It would be a lot
                // less smelly to create a dedicated one
                const std::string cmpName {cmp->GetSubtypeId()};
                ImGui::PushID(cmpName.data());

                if (ImGui::Button("Edit"))
                {
                    cmp->myIsDisplayingWidget = true;
                    cmp->OnStartDisplayWidget();

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

                    if (!cmp->myIsDisplayingWidget)
                    {
                        cmp->OnEndDisplayWidget();
                    }
                }

                ImGui::PopID();
            }
            ImGui::TreePop();
        }
        else
        {
            DrawDeleteEntityButton(uid);
        }
    }
}

void LevelEditorWidget::DrawDeleteEntityButton(const UID& anEntityUID)
{
    ImGui::SameLine();

    if (ImGui::Button("Delete"))
    {
        myEntityToDelete = anEntityUID;
    }
}

void LevelEditorWidget::DrawDeleteEntityPopup()
{
    if (myEntityToDelete == UID::Empty)
    {
        return;
    }

    ImGui::OpenPopup("##DeleteEntity");
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("##DeleteEntity"))
    {
        ImGui::Text("Deleting Entity \'%s\'", myEntityToDelete.GetString().c_str());
        if (ImGui::Button("Confirm"))
        {
            myWorld->RequestEntityUnspawn(myEntityToDelete);

            myEntityToDelete = UID::Empty;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            myEntityToDelete = UID::Empty;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}
