#pragma once

#include "Serializable.h"

struct TileHalfEdge : public Serializable
{
    TileHalfEdge() = default;
    TileHalfEdge(int anIndex, int aHeight, int anIsReversed, int anIsAlternating, int aVertex, int aNext,
                 int anOpposite, int aFace);
    virtual void Serialize(Serializer& aSerializer) override;

    int myIndex {-1};
    int myHeight {-1};

    int myIsReversed {false};
    int myIsAlternating {false};

    int myVertex {-1};
    int myNext {-1};
    int myOpposite {-1};
    int myFace {-1};

    int myParent {-1};
    int myLeftChild {-1};
    int myRightChild {-1};
};
