#pragma once

#include "Filepath.h"
#include "Widget.h"

class WorldModel;

class LevelEditorWidget : public Widget
{
public:
    LevelEditorWidget(WorldModel* aWorldModel);

    void Draw() override;

private:
    void DrawSaveWorld();
    void DrawEntities();

    std::string mySaveWorldName {""};
    bool myShowSaveWorldConfirmation {false};

    WorldModel* const myWorldModel;
};
