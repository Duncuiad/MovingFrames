#include "OpenGLPG_Base.h"

#include "Editor_TransformCmp.h"

#include "Assert.h"
#include "Entity.h"
#include "MathDefines.h"
#include "TransformCmp.h"

#include <cmath>
#include <functional>
#include <glm/ext/matrix_transform.hpp>
#include <glm/matrix.hpp>

REGISTER_SUBTYPE(Editor_TransformCmp)

void Editor_TransformCmp::OnEnterWorld()
{
    myWidget.ConnectOnChangedCallback(std::bind(&Editor_TransformCmp::OnChanged, this));

    const Transform& transform {GetTransformCmp().GetTransform()};
    const Mat3 transform3 {transform};
    const float scale {pow(glm::determinant(transform3), 1.f / 3.f)};
    myWidget.myMovingFrame = MovingFrame(Quat {transform3 / scale}, Vec3 {transform[3]});
    myWidget.myScale = scale;
}

void Editor_TransformCmp::OnChanged() const
{
    Transform transform {Mat4(Mat3(myWidget.myMovingFrame.GetOrientation()))};
    transform *= myWidget.myScale;
    transform[3] = Vec4 {myWidget.myMovingFrame.GetPosition(), 1.f};
    GetTransformCmp().SetTransform(transform);
}

TransformCmp& Editor_TransformCmp::GetTransformCmp() const
{
    TransformCmp* cmp {GetEntity().GetEditableComponent<TransformCmp>()};
    ASSERT(cmp != nullptr, "Couldn't find required component");
    return *cmp;
}
