#pragma once

#include "CameraBehavior.h"

class InputTranslationCameraBehavior : public CameraBehavior
{
public:
    struct Settings
    {
        float mySpeed {0.f};
    };

    InputTranslationCameraBehavior(const Settings& someSettings)
        : mySettings {someSettings}
    {}

    void Update(const UpdateParams& someParams) const override;

private:
    Settings mySettings;
};
