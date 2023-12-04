#include "OpenGLPG_Base.h"

#include "TileMeshCmp.h"

#include "Serializer.h"

REGISTER_SUBTYPE(TileMeshCmp)

void TileMeshCmp::Serialize(Serializer& aSerializer)
{
    aSerializer.Process("myTileMesh", myTileMesh);
}
