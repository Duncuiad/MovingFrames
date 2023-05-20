#include "OpenGLPG_Base.h"

#include "MathUtils.h"

#include "Assert.h"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_inverse.hpp>

namespace Utils
{

glm::mat4 Projection(float aFOV, float anAspectRatio, float aNearPlane, float aFarPlane)
{
    ASSERT(0.f < aFOV && aFOV < glm::pi<float>(), "Invalid FOV for projection matrix: {}", aFOV);
    ASSERT(aNearPlane < aFarPlane, "Invalid plames for projection matrix. Near: {}, Far: {}", aNearPlane, aFarPlane);

    const float g = 1.f / glm::tan(0.5f * aFOV);
    const float s = anAspectRatio;
    const float d = aFarPlane / (aFarPlane - aNearPlane);
    return {{g, 0.f, 0.f, 0.f}, {0.f, g / s, 0.f, 0.f}, {0.f, 0.f, d, 1.f}, {0.f, 0.f, -aNearPlane * d, 0.f}};
}

glm::mat4 WorldToClip(const CameraData& aCameraData)
{
    const glm::mat4 projection {
        Projection(aCameraData.myFOV, aCameraData.myAspectRatio, aCameraData.myNear, aCameraData.myFar)};

    // @note: I can't believe there is no specialization of glm::inverse for rigid motions
    return projection * glm::affineInverse(aCameraData.myCameraTransform);
}

} // namespace Utils
