#include "OpenGLPG_Base.h"

#include "WorldSettings.h"

#include "Serializer.h"

void WorldSettings::Serialize(Serializer& aSerializer)
{
    aSerializer.Process("myCameraAsset", myCameraAsset);
}
