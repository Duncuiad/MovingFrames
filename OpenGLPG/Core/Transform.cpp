#include "OpenGLPG_Base.h"

#include "Transform.h"

Transform::Transform(const Mat4& aMatrix)
    : Base {aMatrix}
{}

Transform::Transform(const Mat3& anOrientation, const Vec3& aPosition)
    : Mat4 {anOrientation}
{
    (*this)[3] = Vec4 {aPosition, 1.f};
}

Transform::Transform(const Quat& anOrientation, const Vec3& aPosition)
    : Transform {Mat3 {anOrientation}, aPosition}
{}
