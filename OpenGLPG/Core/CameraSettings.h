#pragma once

#include <glm/trigonometric.hpp>
#include <string>

enum CameraType {
    FreeCam,
    Orbital,
    Curve
};

class CameraSettings
{
public:
    CameraSettings(CameraType aType)
        : myType {aType}
    {}
    CameraSettings(CameraType aType, float aFOV, float anAspectRatio, float aNearPlane, float aFarPlane)
        : myType {aType}
        , myFOV {aFOV}
        , myAspectRatio {anAspectRatio}
        , myNearPlane {aNearPlane}
        , myFarPlane {aFarPlane}
    {}

private:
    CameraType myType;
    float myFOV {glm::radians(90.f)};
    float myAspectRatio {16.f / 9.f};
    float myNearPlane {0.1f};
    float myFarPlane {200.f};
    friend class CameraFactory;
};
