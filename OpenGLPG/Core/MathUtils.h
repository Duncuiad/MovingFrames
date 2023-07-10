#pragma once

#include "Assert.h"
#include "CameraData.h"
#include "Dual.h"
#include "MathDefines.h"

#include <glm/ext/quaternion_geometric.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/matrix.hpp>
#include <glm/trigonometric.hpp>

Quat Pure(const Vec3& aVector);
float Re(const Quat& aQuaternion);
Vec3 Im(const Quat& aQuaternion);
Dual Norm(const DualQuat& aDualQuaternion);
Dual Norm2(const DualQuat& aDualQuaternion);
DualQuat Normalized(const DualQuat& aDualQuaternion);
DualQuat Exp(const DualQuat& aDualQuaternion);
DualQuat Log(const DualQuat& aDualQuaternion);

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

inline Dual Norm(const DualQuat& aDualQuaternion)
{
    ASSERT(Norm2(aDualQuaternion).IsInvertible(), "Norm is undefined for non invertible dual quaternions");
    const float normReal {glm::length(aDualQuaternion.real)};
    return {normReal, glm::dot(aDualQuaternion.real, aDualQuaternion.dual) / normReal};
}

inline Dual Norm2(const DualQuat& aDualQuaternion)
{
    return {glm::dot(aDualQuaternion.real, aDualQuaternion.real),
            2.f * glm::dot(aDualQuaternion.real, aDualQuaternion.dual)};
}

inline DualQuat Normalized(const DualQuat& aDualQuaternion)
{
    ASSERT(Norm2(aDualQuaternion).IsInvertible(), "Normalizing a non invertible dual quaternion");
    const float normReal2 {glm::dot(aDualQuaternion.real, aDualQuaternion.real)};
    const float invNormReal {glm::inversesqrt(normReal2)};
    const Quat dependentComp {aDualQuaternion.real * glm::dot(aDualQuaternion.real, aDualQuaternion.dual) / normReal2};
    return DualQuat {aDualQuaternion.real, (aDualQuaternion.dual - dependentComp)} * invNormReal;
}

inline DualQuat Exp(const DualQuat& aDualQuaternion)
{
    ASSERT(aDualQuaternion.real.w == 0.f && aDualQuaternion.dual.w == 0.f,
           "Exponentiating a dual quaternion that is not pure");
    const float t {glm::length(aDualQuaternion.real)};
    const float c {glm::dot(aDualQuaternion.real, aDualQuaternion.dual)};
    const float cost {glm::cos(t)};
    const float sint {glm::sin(t)};

    float a {1.f - t * t / 3.f};
    float b {-1.f / 3.f};

    constexpr float expInternalEpsilon {0.01f};
    if (t > expInternalEpsilon)
    {
        a = sint / t;
        b = (cost - a) / (t * t);
    }

    DualQuat result {a * aDualQuaternion};
    result.real.w += cost;
    result.dual.w -= a * c;
    result.dual += b * c * aDualQuaternion.real;
    return result;
}

inline DualQuat Log(const DualQuat& aDualQuaternion)
{
    ASSERT(Norm2(aDualQuaternion).CloseTo(1.f), "Taking the log of a non unitary dual quaternion");

    if (glm::epsilonEqual(Re(aDualQuaternion.real), 1.f, 0.001f))
    {
        // the dual quaternion is a pure translation
        return {Quat {0.f, 0.f, 0.f, 0.f}, aDualQuaternion.dual};
    }

    Vec3 dir {Im(aDualQuaternion.real)};
    const float cos {Re(aDualQuaternion.real)};
    const float sin {glm::length(dir)};
    const float angle {glm::atan(sin, cos)};
    dir = dir / sin;
    const Vec3 vec {Im(aDualQuaternion.dual)};
    const float y0 {Re(aDualQuaternion.dual)};
    const float y1 {glm::dot(vec, dir)};
    const Vec3 normal {vec - y1 * dir};

    const Vec3 real {angle * dir};
    const Vec3 dual {(cos * y1 - sin * y0) * dir + (angle / sin) * normal};
    return {Quat {0.f, real.x, real.y, real.z}, Quat {0.f, dual.x, dual.y, dual.z}};
}
