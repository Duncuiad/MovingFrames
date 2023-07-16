#pragma once

#include "Component.h"
#include "FrameSpline.h"

class FrameSplineCmp : public Component
{
    DECLARE_SUBTYPE(FrameSplineCmp)

public:
    void Serialize(Serializer& aSerializer) override;

    FrameSpline mySpline;
};
