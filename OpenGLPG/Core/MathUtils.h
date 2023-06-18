#pragma once

#include "Assert.h"
#include "CameraData.h"
#include "MathDefines.h"

#include <glm/ext/quaternion_geometric.hpp>
#include <glm/matrix.hpp>
#include <glm/trigonometric.hpp>

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
    ASSERT(aDualQuaternion.real.w == 0.f && aDualQuaternion.dual.w == 0.f,
           "Exponentiating a dual quaternion that is not pure");
    const float t {glm::length(aDualQuaternion.real)};
    const float c {glm::dot(aDualQuaternion.real, aDualQuaternion.dual)};
    const float costh {glm::cos(t * 0.5f)};
    const float sinth {glm::sin(t * 0.5f)};

    float a {0.5f - t * t / 48.f};
    float b {-1.f / 24.f};

    constexpr float expInternalEpsilon {0.01f};
    if (t > expInternalEpsilon)
    {
        a = sinth / t;
        b = (costh * 0.5f - a) / (t * t);
    }

    DualQuat result {a * aDualQuaternion};
    result.real.w += costh;
    result.dual.w -= a * c * 0.5f;
    result.dual += b * c * aDualQuaternion.real;
    return result;
}
