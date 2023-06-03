#pragma once

#include "MathDefines.h"

class MovingFrame
{
public:
    enum Coord {
        Intrinsic,
        Extrinsic
    };

    MovingFrame();
    MovingFrame(const MovingFrame& aMovingFrame) = default;
    MovingFrame(const Quat& anOrientation, const Vec3& aPosition, Vec3 anAngularVelocity = Vec3 {0.f},
                Vec3 aLinearVelocity = Vec3 {0.f});

    Quat GetOrientation() const;
    Vec3 GetPosition() const;
    Vec3 GetAngularVelocity(Coord aCoord) const;
    Vec3 GetLinearVelocity(Coord aCoord) const;
    MovingFrame Inverse() const;

    void ResetVelocities();
    void AddAngularVelocity(Coord aCoord, Vec3 aVelocity);
    void AddLinearVelocity(Coord aCoord, Vec3 aVelocity);
    void Move(float aDeltaTime);

    MovingFrame operator*(const MovingFrame& aFrame) const;
    MovingFrame operator/(const MovingFrame& aFrame) const;

private:
    MovingFrame(const DualQuat& aDualQuaternion);

    DualQuat myPose;
    DualQuat myTwist;
};
