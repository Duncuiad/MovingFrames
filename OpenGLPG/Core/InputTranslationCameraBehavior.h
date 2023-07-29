#pragma once

#include "CameraBehavior.h"

class InputTranslationCameraBehavior : public CameraBehavior
{
    DECLARE_SUBTYPE(InputTranslationCameraBehavior)

public:
    struct Settings
    {
        float mySpeed {0.f};
    };

    InputTranslationCameraBehavior() = default;
    InputTranslationCameraBehavior(const Settings& someSettings)
        : mySettings {someSettings}
    {}

    void Serialize(Serializer& aSerializer) override;

    void Update(const UpdateParams& someParams) const override;

private:
    Settings mySettings;
};
