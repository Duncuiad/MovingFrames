#pragma once

#include "Component.h"
#include "Transform.h"

class TransformCmp : public Component
{
    const Transform& GetTransform() const;
    void SetTransform(const Transform& aTransform);

private:
    Transform myTransform;
};

inline const Transform& TransformCmp::GetTransform() const
{
    return myTransform;
}

inline void TransformCmp::SetTransform(const Transform& aTransform)
{
    myTransform = aTransform;
}
