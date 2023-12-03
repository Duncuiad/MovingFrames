#include "OpenGLPG_Base.h"

#include "TileFace.h"

TileFace::TileFace(int anIndex, int aHeight, TileType aType, int anEdge)
    : myIndex {anIndex}
    , myHeight {aHeight}
    , myType {aType}
    , myEdge {anEdge}
{}

void TileFace::Serialize(Serializer& aSerializer) {}
