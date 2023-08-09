#include "OpenGLPG_Base.h"

#include "Editor_TransformCmp.h"

#include "Assert.h"
#include "Entity.h"
#include "MathDefines.h"
#include "TransformCmp.h"

#include <functional>
#include <glm/ext/matrix_transform.hpp>

REGISTER_SUBTYPE(Editor_TransformCmp)

void Editor_TransformCmp::OnEnterWorld()
{
    myWidget.ConnectOnChangedCallback(std::bind(&Editor_TransformCmp::OnChanged, this));

    const Transform& transform {GetTransformCmp().GetTransform()};
    myWidget.myMovingFrame = MovingFrame(Quat {Mat3 {transform}}, Vec3 {transform[3]});
}

void Editor_TransformCmp::OnChanged() const
{
    Transform transform {Mat4(Mat3(myWidget.myMovingFrame.GetOrientation()))};
    transform[3] = Vec4 {myWidget.myMovingFrame.GetPosition(), 1.f};
    GetTransformCmp().SetTransform(transform);
}

TransformCmp& Editor_TransformCmp::GetTransformCmp() const
{
    TransformCmp* cmp {GetEntity().GetEditableComponent<TransformCmp>()};
    ASSERT(cmp != nullptr, "Couldn't find required component");
    return *cmp;
}
