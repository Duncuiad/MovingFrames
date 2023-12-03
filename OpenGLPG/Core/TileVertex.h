#pragma once

// #include "Array.h"
#include "MathDefines.h"
#include "Serializable.h"

struct TileVertex : public Serializable
{
    TileVertex() = default;
    TileVertex(int anIndex, int aHeight, const Vec2& aPosition);
    virtual void Serialize(Serializer& aSerializer) override;

    int myIndex {-1};
    int myHeight {-1};

    Vec2 myPosition {0.f, 0.f};

    // Array<Array<int>> myEdgeFans;
};
