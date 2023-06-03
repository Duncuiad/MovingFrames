#include "OpenGLPG_Base.h"

#include "MovingFrame.h"

#include "MathUtils.h"

#include <glm/ext/quaternion_exponential.hpp>

MovingFrame::MovingFrame()
    : myPose {}
    , myTwist {}
{
    // dual part is orthogonal to real part (which is the identity)
    myPose.dual.w = 0.f;
}

MovingFrame::MovingFrame(const Quat& anOrientation, const Vec3& aPosition, Vec3 anAngularVelocity /*= Vec3 {0.f}*/,
                         Vec3 aLinearVelocity /*= Vec3 {0.f}*/)
    : myPose {anOrientation, aPosition}
    , myTwist {Pure(anAngularVelocity), Pure(aLinearVelocity)} // @todo: decide how the twist should be stored
{}

Quat MovingFrame::GetOrientation() const
{
    return myPose.real;
}

Vec3 MovingFrame::GetPosition() const
{
    return Im(myPose.dual * glm::conjugate(myPose.real)) * 2.f;
}

Vec3 MovingFrame::GetAngularVelocity(Coord aCoord) const
{
    // @todo: decide how the twist should be stored
    return Im(myTwist.real);
}

Vec3 MovingFrame::GetLinearVelocity(Coord aCoord) const
{
    // @todo: decide how the twist should be stored
    return Im(myTwist.dual);
}

MovingFrame MovingFrame::Inverse() const
{
    // @todo: Implement inverse
    return MovingFrame();
}

void MovingFrame::ResetVelocities()
{
    myTwist = DualQuat {Quat {0.f, 0.f, 0.f, 0.f}, Quat {0.f, 0.f, 0.f, 0.f}};
}

void MovingFrame::AddAngularVelocity(Coord aCoord, Vec3 aVelocity)
{
    // @todo: decide how the twist should be stored
    switch (aCoord)
    {
    case Coord::Extrinsic:
        myTwist.real += Pure(aVelocity);
        break;
    case Coord::Intrinsic:
        myTwist.real += Pure(myPose.real * aVelocity);
    }
}

void MovingFrame::AddLinearVelocity(Coord aCoord, Vec3 aVelocity)
{
    // @todo: decide how the twist should be stored
    switch (aCoord)
    {
    case Coord::Extrinsic:
        myTwist.dual += Pure(aVelocity);
        break;
    case Coord::Intrinsic:
        myTwist.dual += Pure(myPose.real * aVelocity);
    }
}

void MovingFrame::Move(float aDeltaTime)
{
    // @todo: decide how the twist should be stored
    // @todo: implement dual quaternion exponential
    const DualQuat step {aDeltaTime * myTwist};
    myPose = Exp(step) * myPose;
}

MovingFrame MovingFrame::operator*(const MovingFrame& aFrame) const
{
    MovingFrame result {myPose * aFrame.myPose};
    // @todo: decide how the twist should be stored
    result.myTwist.real = myTwist.real + myPose.real * aFrame.myTwist.real;
    result.myTwist.dual = myTwist.dual + myPose.real * aFrame.myTwist.dual;
    return result;
}

MovingFrame MovingFrame::operator/(const MovingFrame& aFrame) const
{
    return *this * aFrame.Inverse();
}

MovingFrame::MovingFrame(const DualQuat& aDualQuaternion)
    : myPose {aDualQuaternion}
{}
