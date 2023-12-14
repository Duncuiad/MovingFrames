#include "OpenGLPG_Base.h"

#include "TileVertex.h"

#include "Serializer.h"

TileVertex::TileVertex(int anIndex, int aHeight, const Dodec& aDodec)
    : myIndex {anIndex}
    , myHeight {aHeight}
    , myCoordinates {aDodec}
{}

void TileVertex::Serialize(Serializer& aSerializer)
{
    aSerializer.Process("myIndex", myIndex);
    aSerializer.Process("myHeight", myHeight);
    aSerializer.Process("myCoordinates", myCoordinates);
    aSerializer.Process("myData", myData);
}

const Vec2& TileVertex::GetPosition() const
{
    if (!myCachedPosition)
    {
        myCachedPosition = myCoordinates.GetPos();
    }
    return *myCachedPosition;
}

void TileVertex::Data::Serialize(Serializer& aSerializer)
{
    aSerializer.Process("myColor", myColor);
}
