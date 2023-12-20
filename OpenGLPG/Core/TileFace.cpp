#include "OpenGLPG_Base.h"

#include "TileFace.h"

#include "Serializer.h"
#include "TileHalfEdge.h"

TileFace::TileFace(int anIndex, int aHeight, TileType aType, int anEdge)
    : myIndex {anIndex}
    , myHeight {aHeight}
    , myType {aType}
    , myEdge {anEdge}
{}

void TileFace::Serialize(Serializer& aSerializer)
{
    aSerializer.Process("myIndex", myIndex);
    aSerializer.Process("myHeight", myHeight);
    aSerializer.Process("myEdge", myEdge);

    int type {static_cast<int>(myType)};
    aSerializer.Process("myType", type);
    myType = static_cast<TileType>(type);

    aSerializer.Process("myParent", myParent);
    aSerializer.Process("myData", myData);
}

bool TileFace::IsTriangle() const
{
    return myType == TileType::TriangleA || myType == TileType::TriangleB;
}

bool TileFace::IsSquare() const
{
    return myType == TileType::SquareA || myType == TileType::SquareB || myType == TileType::SquareC;
}

void TileFace::Data::Serialize(Serializer& aSerializer)
{
    aSerializer.Process("myColor", myColor);
}
