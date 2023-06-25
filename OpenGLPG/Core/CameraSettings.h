#pragma once

#include "Filepath.h"

#include <glm/trigonometric.hpp>
#include <string>

enum CameraType {
    FileBased,
    FreeCam,
    Orbital,
    Curve
};

class CameraSettings
{
public:
    CameraSettings(const Filepath& anAsset)
        : myType {CameraType::FileBased}
        , myAsset {anAsset}
    {}
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
    Filepath myAsset {};
    friend class CameraFactory;
};
