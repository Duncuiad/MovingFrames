#include "OpenGLPG_Base.h"

#include "TileVertex.h"

TileVertex::TileVertex(int anIndex, int aHeight, const Vec2& aPosition)
    : myIndex {anIndex}
    , myHeight {aHeight}
    , myPosition {aPosition}
{}

void TileVertex::Serialize(Serializer& aSerializer) {}
