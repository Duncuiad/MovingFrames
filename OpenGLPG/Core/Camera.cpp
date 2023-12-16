#include "OpenGLPG_Base.h"

#include "Camera.h"

void Camera::Serialize(Serializer& aSerializer)
{
    aSerializer.Process("myBehaviors", myBehaviors);
    aSerializer.Process("myFOV", myData.myFOV);
    aSerializer.Process("myOrthoSize", myData.myOrthoSize);
    aSerializer.Process("myAspectRatio", myData.myAspectRatio);
    aSerializer.Process("myNearPlane", myData.myNearPlane);
    aSerializer.Process("myFarPlane", myData.myFarPlane);

    myData.myIsOrtho = myData.myOrthoSize > 0.f;
}

Camera::Camera(const UID& aCameraUID)
    : myUID {aCameraUID}
{}
