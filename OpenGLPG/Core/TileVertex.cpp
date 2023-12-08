#include "OpenGLPG_Base.h"

#include "TileVertex.h"

#include "Serializer.h"

TileVertex::TileVertex(int anIndex, int aHeight, const Vec2& aPosition)
    : myIndex {anIndex}
    , myHeight {aHeight}
{
    myData.myPosition = aPosition;
}

void TileVertex::Serialize(Serializer& aSerializer)
{
    aSerializer.Process("myIndex", myIndex);
    aSerializer.Process("myHeight", myHeight);
    aSerializer.Process("myData", myData);
}

void TileVertex::Data::Serialize(Serializer& aSerializer)
{
    aSerializer.Process("myPosition", myPosition);
    aSerializer.Process("myColor", myColor);
}
