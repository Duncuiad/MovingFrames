#include "OpenGLPG_Base.h"

#include "WorldSettings.h"

#include "Serializer.h"

void WorldSettings::Serialize(Serializer& aSerializer)
{
    aSerializer.Process("myCameraAsset", myCameraAsset);

    {
        bool hasBackgroundColor {myBackgroundColor.has_value()};
        aSerializer.Process("myHasBackgroundColor", hasBackgroundColor);
        if (hasBackgroundColor)
        {
            Vec3 backgroundColor {myBackgroundColor.has_value() ? *myBackgroundColor : Vec3 {0.f}};
            aSerializer.Process("myBackgroundColor", backgroundColor);
            myBackgroundColor = backgroundColor;
        }
    }
}
