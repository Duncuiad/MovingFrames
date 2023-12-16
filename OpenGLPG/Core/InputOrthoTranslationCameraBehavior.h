#pragma once

#include "CameraBehavior.h"

class InputOrthoTranslationCameraBehavior : public CameraBehavior
{
    DECLARE_SUBTYPE(InputOrthoTranslationCameraBehavior)

public:
    struct Settings
    {
        float mySpeed {0.f};
    };

    InputOrthoTranslationCameraBehavior() = default;
    InputOrthoTranslationCameraBehavior(const Settings& someSettings)
        : mySettings {someSettings}
    {}

    void Serialize(Serializer& aSerializer) override;

    void Update(const UpdateParams& someParams) const override;

private:
    Settings mySettings;
};
