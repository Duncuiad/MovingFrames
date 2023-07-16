#pragma once

#include "DebugImGui.h"
#include "DebugUtils.h"
#include "MathDefines.h"
#include "Serializable.h"

class MovingFrame : public Serializable
{
public:
    enum Coord {
        Intrinsic,
        Extrinsic
    };

    MovingFrame();
    MovingFrame(const MovingFrame& aMovingFrame) = default;
    explicit MovingFrame(const DualQuat& aDualQuaternion);
    MovingFrame(const Quat& anOrientation, const Vec3& aPosition, Vec3 anAngularVelocity = Vec3 {0.f},
                Vec3 aLinearVelocity = Vec3 {0.f});

    void Serialize(Serializer& aSerializer) override;

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
#if DEBUG_IMGUI
    friend void ::ImGui::Draw(const MovingFrame&);
#endif

    DualQuat myPose;
    DualQuat myTwist;
};
