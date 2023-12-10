#pragma once

#include "Game.h"
#include "LevelEditorWidget.h"

class LevelEditor : public Game
{
public:
    using Base = Game;
    explicit LevelEditor(const ConstructionParams& someParams);

    void Update(const UpdateParams& someParams) override;

private:
    void HandleInput(const UpdateParams& someParams);

    LevelEditorWidget myWidget;
};
