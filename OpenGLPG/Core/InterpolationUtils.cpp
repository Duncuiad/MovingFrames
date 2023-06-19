#include "OpenGLPG_Base.h"

#include "InterpolationUtils.h"

#include "Assert.h"
#include "MathUtils.h"

DualQuat Sclerp(const DualQuat& aPose0, const DualQuat& aPose1, float aT)
{
    ASSERT(Norm2(aPose0).IsInvertible(), "Source pose in a non invertible dual quaternion");
    return Normalized(Exp(aT * Log(aPose1 * glm::inverse(aPose0))) * aPose0);
}

DualQuat Sclerp(const DualQuat& aPose0, const DualQuat& aPose1, float aT, const DualQuat& aMonodromyReference)
{
    const DualQuat result {Sclerp(aPose0, aPose1, aT)};
    return glm::dot(result.real, aMonodromyReference.real) < 0.f ? -result : result;
}
