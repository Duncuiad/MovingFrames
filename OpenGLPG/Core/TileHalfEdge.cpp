#include "OpenGLPG_Base.h"

#include "TileHalfEdge.h"

#include "Serializer.h"

TileHalfEdge::TileHalfEdge(int anIndex, int aHeight, bool anIsReversed, bool anIsAlternating, const Dodec& aVertex,
                           int aNext, int anOpposite, int aFace)
    : myIndex {anIndex}
    , myHeight {aHeight}
    , myIsReversed {anIsReversed}
    , myIsAlternating {anIsAlternating}
    , myVertex {aVertex}
    , myNext {aNext}
    , myOpposite {anOpposite}
    , myFace {aFace}
{}

void TileHalfEdge::Serialize(Serializer& aSerializer)
{
    aSerializer.Process("myIndex", myIndex);
    aSerializer.Process("myHeight", myHeight);
    aSerializer.Process("myIsReversed", myIsReversed);
    aSerializer.Process("myIsAlternating", myIsAlternating);
    aSerializer.Process("myVertex", myVertex);
    aSerializer.Process("myNext", myNext);
    aSerializer.Process("myOpposite", myOpposite);
    aSerializer.Process("myFace", myFace);
}
