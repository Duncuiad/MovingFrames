#include "OpenGLPG_Base.h"

#include "MathUtils.h"

#include "Assert.h"
#include "Transform.h" // Mat4 glm::affineTransform(const Transform&)

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace Utils
{
glm::mat4 Ortho(float anOrthoSize, float anAspectRatio, float aNearPlane, float aFarPlane)
{
    ASSERT(anOrthoSize > 0.f, "Invalid Ortho Size for projection matrix: {}", anOrthoSize);
    ASSERT(aNearPlane < aFarPlane, "Invalid plames for projection matrix. Near: {}, Far: {}", aNearPlane, aFarPlane);
    const Vec2 orthoSize {anOrthoSize * anAspectRatio, anOrthoSize};
    return glm::ortho(-orthoSize.x, orthoSize.x, -orthoSize.y, orthoSize.y, aNearPlane, aFarPlane);
}

glm::mat4 Projection(float aFOVy, float anAspectRatio, float aNearPlane, float aFarPlane)
{
    ASSERT(0.f < aFOVy && aFOVy < glm::pi<float>(), "Invalid FOV for projection matrix: {}", aFOVy);
    ASSERT(aNearPlane < aFarPlane, "Invalid plames for projection matrix. Near: {}, Far: {}", aNearPlane, aFarPlane);
    return glm::perspective(aFOVy, anAspectRatio, aNearPlane, aFarPlane);
}

glm::mat4 Projection(const CameraData& aCameraData)
{
    return aCameraData.myIsOrtho
               ? Ortho(aCameraData.myOrthoSize, aCameraData.myAspectRatio, aCameraData.myNear, aCameraData.myFar)
               : Projection(aCameraData.myFOVy, aCameraData.myAspectRatio, aCameraData.myNear, aCameraData.myFar);
}

glm::mat4 WorldToClip(const CameraData& aCameraData)
{
    // @note: I can't believe there is no specialization of glm::inverse for rigid motions
    return Projection(aCameraData) * glm::affineInverse(aCameraData.myCameraTransform);
}

} // namespace Utils
