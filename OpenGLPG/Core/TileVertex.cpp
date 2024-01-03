#include "OpenGLPG_Base.h"

#include "TileVertex.h"

#include "Serializer.h"

TileVertex::TileVertex(int aHeight)
    : myHeight {aHeight}
{}

void TileVertex::Serialize(Serializer& aSerializer)
{
    aSerializer.Process("myHeight", myHeight);
    aSerializer.Process("myData", myData);
}

void TileVertex::Data::Serialize(Serializer& aSerializer)
{
    aSerializer.Process("myColor", myColor);
}
