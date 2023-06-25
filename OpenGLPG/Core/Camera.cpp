#include "OpenGLPG_Base.h"

#include "Camera.h"

void Camera::Serialize(Serializer& aSerializer)
{
    aSerializer.Process("myBehaviors", myBehaviors);
    aSerializer.Process("myFOV", myData.myFOV);
    aSerializer.Process("myAspectRatio", myData.myAspectRatio);
    aSerializer.Process("myNearPlane", myData.myNearPlane);
    aSerializer.Process("myFarPlane", myData.myFarPlane);
}

Camera::Camera(const UID& aCameraUID)
    : myUID {aCameraUID}
{}
