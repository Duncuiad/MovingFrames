#include "OpenGLPG_Base.h"

#include "TileVertex.h"

#include "Serializer.h"

TileVertex::TileVertex(int anIndex, int aHeight, const Vec2& aPosition)
    : myIndex {anIndex}
    , myHeight {aHeight}
    , myPosition {aPosition}
{}

void TileVertex::Serialize(Serializer& aSerializer)
{
    aSerializer.Process("myIndex", myIndex);
    aSerializer.Process("myHeight", myHeight);
    aSerializer.Process("myPosition", myPosition);
}
