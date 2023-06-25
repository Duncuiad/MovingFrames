#include "OpenGLPG_Base.h"

#include "TransformCmp.h"

#include "Serializer.h"

REGISTER_SUBTYPE(TransformCmp)

void TransformCmp::Serialize(Serializer& aSerializer)
{
    aSerializer.Process("myTransform", myTransform);
}

const Transform& TransformCmp::GetTransform() const
{
    return myTransform;
}

void TransformCmp::SetTransform(const Transform& aTransform)
{
    myTransform = aTransform;
}
