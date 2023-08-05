#include "OpenGLPG_Base.h"

#include "MovingFrame.h"

#include "MathUtils.h"
#include "Serializer.h"

#include <glm/ext/quaternion_exponential.hpp>

MovingFrame::MovingFrame()
    : myPose {}
    , myTwist {}
{
    // real part is identity
    myPose.real.w = 1.f;
}

MovingFrame::MovingFrame(const DualQuat& aDualQuaternion, DualQuat aTwist)
    : myPose {aDualQuaternion}
    , myTwist {aTwist}
{}

MovingFrame::MovingFrame(const Quat& anOrientation, const Vec3& aPosition, Vec3 anAngularVelocity /*= Vec3 {0.f}*/,
                         Vec3 aLinearVelocity /*= Vec3 {0.f}*/)
    : myPose {anOrientation, aPosition}
    , myTwist {Pure(anAngularVelocity), Pure(aLinearVelocity)}
{}

void MovingFrame::Serialize(Serializer& aSerializer)
{
    aSerializer.Process("myPoseReal", myPose.real);
    aSerializer.Process("myPoseDual", myPose.dual);
    aSerializer.Process("myTwistReal", myTwist.real);
    aSerializer.Process("myTwistDual", myTwist.dual);
}

const DualQuat& MovingFrame::GetPose() const
{
    return myPose;
}

const DualQuat& MovingFrame::GetTwist() const
{
    return myTwist;
}

Quat MovingFrame::GetOrientation() const
{
    return myPose.real;
}

Vec3 MovingFrame::GetPosition() const
{
    return Im(myPose.dual * glm::conjugate(myPose.real)) * 2.f;
}

Vec3 MovingFrame::GetAngularVelocity(Coord /*aCoord*/) const
{
    // @todo: implement Coord selection
    return Im(myTwist.real);
}

Vec3 MovingFrame::GetLinearVelocity(Coord /*aCoord*/) const
{
    // @todo: implement Coord selection
    return Im(myTwist.dual);
}

MovingFrame MovingFrame::Inverse() const
{
    // @todo: Implement inverse
    return MovingFrame();
}

void MovingFrame::SetOrientation(const Quat& anOrientation)
{
    myPose.real = anOrientation;
}

void MovingFrame::SetPosition(const Vec3& aPosition)
{
    Quat quat {0.f, aPosition.x, aPosition.y, aPosition.z};
    myPose.dual = (quat * myPose.real) * 0.5f;
}

void MovingFrame::SetAngularVelocity(const Vec3& aVelocity, Coord /*aCoord*/)
{
    // @todo: implement Coord selection
    myTwist.real = Quat {0.f, aVelocity.x, aVelocity.y, aVelocity.z};
}

void MovingFrame::SetLinearVelocity(const Vec3& aVelocity, Coord /*aCoord*/)
{
    // @todo: implement Coord selection
    myTwist.dual = Quat {0.f, aVelocity.x, aVelocity.y, aVelocity.z};
}

void MovingFrame::ResetVelocities()
{
    myTwist = DualQuat {Quat {0.f, 0.f, 0.f, 0.f}, Quat {0.f, 0.f, 0.f, 0.f}};
}

void MovingFrame::AddAngularVelocity(Coord aCoord, Vec3 aVelocity)
{
    switch (aCoord)
    {
    case Coord::Extrinsic:
        myTwist.real += Pure(glm::conjugate(myPose.real) * aVelocity);
        break;
    case Coord::Intrinsic:
        myTwist.real += Pure(aVelocity);
        break;
    }
}

void MovingFrame::AddLinearVelocity(Coord aCoord, Vec3 aVelocity)
{
    switch (aCoord)
    {
    case Coord::Extrinsic:
        myTwist.dual += Pure(glm::conjugate(myPose.real) * aVelocity);
        break;
    case Coord::Intrinsic:
        myTwist.dual += Pure(aVelocity);
        break;
    }
}

void MovingFrame::Move(float aDeltaTime)
{
    const DualQuat step {aDeltaTime * myTwist};
    myPose = Normalized(myPose * Exp(0.5f * step));
}

MovingFrame MovingFrame::operator*(const MovingFrame& aFrame) const
{
    MovingFrame result {myPose * aFrame.myPose};
    result.myTwist.real = glm::conjugate(myPose.real) * myTwist.real + aFrame.myTwist.real;
    result.myTwist.dual = glm::conjugate(myPose.real) * myTwist.dual + aFrame.myTwist.dual;
    // @todo: check calc
    return result;
}

MovingFrame MovingFrame::operator/(const MovingFrame& aFrame) const
{
    return *this * aFrame.Inverse();
}
