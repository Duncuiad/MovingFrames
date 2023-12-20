#include "OpenGLPG_Base.h"

#include "LevelEditor.h"

#include "CameraData.h"
#include "CollisionSystem.h"
#include "CollisionTag.h"
#include "MathDefines.h"
#include "Transform.h"

LevelEditor::LevelEditor(const ConstructionParams& someParams)
    : Base {someParams}
    , myWidget {&myWorld, someParams.myLoader.myTemplateFolder, "main.templateindex"}
{}

void LevelEditor::Update(const UpdateParams& someParams)
{
    HandleInput(someParams);
    Base::Update(someParams);
    myWidget.Draw();
}

void LevelEditor::HandleInput(const UpdateParams& someParams)
{
    CollisionTag tag {CollisionTag::None};
    const GameInputData::InputContainer& inputs {myInputData.myInputs};
    if (inputs.at(GameInput::theMouseClickLeft) > 0.f)
    {
        tag = CollisionTag::PickLeftClick;
    }
    else if (inputs.at(GameInput::theMouseClickRight) > 0.f)
    {
        tag = CollisionTag::PickRightClick;
    }

    if (tag != CollisionTag::None)
    {
        const Vec2 mousePosition {inputs.at(GameInput::theMousePosX), inputs.at(GameInput::theMousePosY)};
        if (mousePosition.x >= 0.f && mousePosition.y >= 0.f)
        {
            const Vec2 viewportSize {someParams.myViewportBR - someParams.myViewportTL};
            const Vec2 clipPos {mousePosition / viewportSize * 2.f - Vec2 {1.f}};
            const CameraData& data {myWorld.GetActiveCameraData()};
            const Transform& matrix {data.myCameraTransform};
            if (data.myIsOrtho)
            {
                const Vec3 position {matrix[3] +
                                     clipPos.x * (viewportSize.x / viewportSize.y) * data.myOrthoSize * matrix[0] +
                                     -clipPos.y * matrix[1] * data.myOrthoSize};
                myWorld.GetCollisionSystem().RayCast(position, -Vec3(matrix[2]), tag);
            }
            else
            {
                const Vec3 rayVector {clipPos.x * (viewportSize.x / viewportSize.y) * tan(data.myFOVy * 0.5f) *
                                          matrix[0] +
                                      -clipPos.y * tan(data.myFOVy * 0.5f) * matrix[1] - matrix[2]};
                myWorld.GetCollisionSystem().RayCast(Vec3(matrix[3]), rayVector, tag);
            }
        }
    }
}
