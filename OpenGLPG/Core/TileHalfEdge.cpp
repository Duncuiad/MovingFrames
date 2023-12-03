#include "OpenGLPG_Base.h"

#include "TileHalfEdge.h"

TileHalfEdge::TileHalfEdge(int anIndex, int aHeight, int anIsReversed, int anIsAlternating, int aVertex, int aNext,
                           int anOpposite, int aFace)
    : myIndex {anIndex}
    , myHeight {aHeight}
    , myIsReversed {anIsReversed}
    , myIsAlternating {anIsAlternating}
    , myVertex {aVertex}
    , myNext {aNext}
    , myOpposite {anOpposite}
    , myFace {aFace}
{}

void TileHalfEdge::Serialize(Serializer& aSerializer) {}
