#include "OpenGLPG_Base.h"

#include "LevelEditor.h"

LevelEditor::LevelEditor(const ConstructionParams& someParams)
    : Base {someParams}
    , myWidget {&myWorldModel, someParams.myLoader.myTemplateFolder, "main.templateindex"}
{}

void LevelEditor::Update(const UpdateParams& someParams)
{
    Base::Update(someParams);
    myWidget.Draw();
}
