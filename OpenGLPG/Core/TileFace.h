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

    bool IsTriangle() const;
    bool IsSquare() const;

    int myIndex {-1};
    int myHeight {-1};

    TileType myType {TileType::SquareA};
    int myEdge {-1};

    int myParent {-1};
    Array<int> myChildren;

    struct Data
    {
        bool myColor {false};
    };

    Data myData;
};
