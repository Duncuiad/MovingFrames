#pragma once

#include "CameraData.h"

#include <glm/matrix.hpp>

namespace Utils
{
glm::mat4 Projection(float aFOV, float anAspectRatio, float aNearPlane, float aFarPlane);
glm::mat4 WorldToClip(const CameraData& aCameraData);
} // namespace Utils
