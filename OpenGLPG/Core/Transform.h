#pragma once

#include "MathDefines.h"
#include "Serializable.h"

#include <glm/gtc/matrix_inverse.hpp>

class Transform : public Mat4, public Serializable
{
public:
    using Base = Mat4;
    using Base::Base;
    Transform(const Mat4& aMatrix);
    Transform(const Mat3& anOrientation, const Vec3& aPosition);
    Transform(const Quat& anOrientation, const Vec3& aPosition);

    void Serialize(Serializer& aSerializer) override;
};

namespace glm
{
inline mat4 affineInverse(const Transform& aTransform)
{
    return affineInverse<float, highp>(aTransform);
}
} // namespace glm
