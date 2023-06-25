#pragma once

#include "Component.h"
#include "Transform.h"

class TransformCmp : public Component
{
    DECLARE_SUBTYPE(TransformCmp)

public:
    void Serialize(Serializer& aSerializer) override;

    const Transform& GetTransform() const;
    void SetTransform(const Transform& aTransform);

private:
    Transform myTransform;
};
