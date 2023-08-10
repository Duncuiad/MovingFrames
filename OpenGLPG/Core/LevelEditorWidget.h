#pragma once

#include "Filepath.h"
#include "UID.h"
#include "Widget.h"

#include <string>

class World;

class LevelEditorWidget : public Widget
{
public:
    LevelEditorWidget(World* aWorld, const Filepath& anEntityTemplateFolder, const Filepath& anEntityTemplateIndex);

    void Draw() override;

private:
    void DrawSaveWorld();
    void DrawAddEntity();
    void DrawEntities();
    void DrawDeleteEntityButton(const UID& anEntityUID);
    void DrawDuplicateEntityButton(const UID& anEntityUID);
    void DrawDeleteEntityPopup();
    void DrawDuplicateEntityPopup();

    std::string mySaveWorldName {""};
    bool myShowSaveWorldConfirmation {false};
    UID myEntityToDelete {UID::Empty};
    UID myEntityToDuplicate {UID::Empty};
    UID myNewEntityDuplicate {UID::Empty};

    World* const myWorld;
    const Filepath myEntityTemplateFolder;
    Array<Filepath> myEntityTemplates;
};
