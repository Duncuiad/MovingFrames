#pragma once

#include "Filepath.h"
#include "UID.h"
#include "Widget.h"

#include <string>

class WorldModel;

class LevelEditorWidget : public Widget
{
public:
    LevelEditorWidget(WorldModel* aWorldModel, const Filepath& anEntityTemplateFolder,
                      const Filepath& anEntityTemplateIndex);

    void Draw() override;

private:
    void DrawSaveWorld();
    void DrawAddEntity();
    void DrawEntities();
    void DrawDeleteEntityButton(const UID& anEntityUID);
    void DrawDeleteEntityPopup();

    std::string mySaveWorldName {""};
    bool myShowSaveWorldConfirmation {false};
    UID myEntityToDelete {UID::Empty};

    WorldModel* const myWorldModel;
    const Filepath myEntityTemplateFolder;
    Array<Filepath> myEntityTemplates;
};
