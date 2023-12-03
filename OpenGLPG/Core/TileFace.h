#pragma once

#include "Array.h"
#include "Serializable.h"

enum class TileType {
    TriangleA,
    TriangleB,
    SquareA,
    SquareB,
    SquareC
};

struct TileFace : public Serializable
{
    TileFace() = default;
    TileFace(int anIndex, int aHeight, TileType aType, int anEdge);
    virtual void Serialize(Serializer& aSerializer) override;

    int myIndex {-1};
    int myHeight {-1};

    TileType myType;
    int myEdge;

    int myParent {-1};
    Array<int> myChildren;
};
