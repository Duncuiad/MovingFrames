#pragma once

#include "CameraBehavior.h"

class InputRotationCameraBehavior : public CameraBehavior
{
    DECLARE_SUBTYPE(InputRotationCameraBehavior)

public:
    struct Settings
    {
        float myHorizontalSpeed {0.f};
        float myVerticalSpeed {0.f};
    };

    InputRotationCameraBehavior(const Settings& someSettings)
        : mySettings {someSettings}
    {}

    void Serialize(Serializer& aSerializer) override;

    void Update(const UpdateParams& someParams) const override;

private:
    Settings mySettings;
};
