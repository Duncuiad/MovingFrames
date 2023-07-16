#include "OpenGLPG_Base.h"

#include "FrameSplineCmp.h"

#include "Serializer.h"

REGISTER_SUBTYPE(FrameSplineCmp)

void FrameSplineCmp::Serialize(Serializer& aSerializer)
{
    aSerializer.Process("mySpline", mySpline);
}
