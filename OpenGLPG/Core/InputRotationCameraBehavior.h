#pragma once

#include "CameraBehavior.h"

class InputRotationCameraBehavior : public CameraBehavior
{
public:
    struct Settings
    {
        float myHorizontalSpeed {0.f};
        float myVerticalSpeed {0.f};
    };

    InputRotationCameraBehavior(const Settings& someSettings)
        : mySettings {someSettings}
    {}

    void Update(const UpdateParams& someParams) const override;

private:
    Settings mySettings;
};
