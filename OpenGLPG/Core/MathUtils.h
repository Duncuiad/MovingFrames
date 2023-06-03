#pragma once

#include "CameraData.h"
#include "MathDefines.h"

#include <glm/matrix.hpp>

Quat Pure(const Vec3& aVector);
float Re(const Quat& aQuaternion);
Vec3 Im(const Quat& aQuaternion);
DualQuat Exp(const DualQuat& aDualQuaternion);

namespace Utils
{
glm::mat4 Projection(float aFOV, float anAspectRatio, float aNearPlane, float aFarPlane);
glm::mat4 WorldToClip(const CameraData& aCameraData);

} // namespace Utils

// inline implementations

inline Quat Pure(const Vec3& aVector)
{
    return {0.f, aVector.x, aVector.y, aVector.z};
}

inline float Re(const Quat& aQuaternion)
{
    return aQuaternion.w;
}

inline Vec3 Im(const Quat& aQuaternion)
{
    return {aQuaternion.x, aQuaternion.y, aQuaternion.z};
}

inline DualQuat Exp(const DualQuat& aDualQuaternion)
{
    // @todo: implement dual quaternion exponential
    return DualQuat();
}
