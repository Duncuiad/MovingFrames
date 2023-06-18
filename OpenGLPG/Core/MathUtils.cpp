#include "OpenGLPG_Base.h"

#include "MathUtils.h"

#include "Assert.h"
#include "Transform.h" // Mat4 glm::affineTransform(const Transform&)

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace Utils
{

glm::mat4 Projection(float aFOV, float anAspectRatio, float aNearPlane, float aFarPlane)
{
    ASSERT(0.f < aFOV && aFOV < glm::pi<float>(), "Invalid FOV for projection matrix: {}", aFOV);
    ASSERT(aNearPlane < aFarPlane, "Invalid plames for projection matrix. Near: {}, Far: {}", aNearPlane, aFarPlane);
    return glm::perspective(aFOV / anAspectRatio, anAspectRatio, aNearPlane, aFarPlane);
}

glm::mat4 WorldToClip(const CameraData& aCameraData)
{
    const glm::mat4 projection {
        Projection(aCameraData.myFOV, aCameraData.myAspectRatio, aCameraData.myNear, aCameraData.myFar)};

    // @note: I can't believe there is no specialization of glm::inverse for rigid motions
    return projection * glm::affineInverse(aCameraData.myCameraTransform);
}

} // namespace Utils
