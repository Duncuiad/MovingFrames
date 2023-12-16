#pragma once

#include "CameraBehavior.h"

class InputOrthoSizeCameraBehavior : public CameraBehavior
{
    DECLARE_SUBTYPE(InputOrthoSizeCameraBehavior)

public:
    struct Settings
    {
        float mySpeed {0.f};
    };

    InputOrthoSizeCameraBehavior() = default;
    InputOrthoSizeCameraBehavior(const Settings& someSettings)
        : mySettings {someSettings}
    {}

    void Serialize(Serializer& aSerializer) override;

    void Update(const UpdateParams& someParams) const override;

private:
    Settings mySettings;
};
