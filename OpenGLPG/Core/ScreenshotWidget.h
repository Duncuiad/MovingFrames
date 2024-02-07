#pragma once

#include <array>
#include <string>

class ScreenshotWidget
{
public:
    void DrawWidget();
    void DrawResolutionSelection(float aMaxTextureSize);
    void ActivateWidget();
    bool ShouldTakeScreenshot() const;
    int GetMaxWidth() const;
    int GetMaxHeight() const;
    int GetWidth() const;
    int GetHeight() const;

private:
    enum class Strategy {
        Preset,
        Custom
    };
    enum class Resolution {
        R_1K,
        R_2K,
        R_4K
    };
    enum class AspectRatio {
        A_21_9,
        A_16_9,
        A_4_3,
        A_1_1,
        A_3_4,
        A_9_16,
        A_9_21
    };

    static int GetMaxTextureSize();
    static int GetHeight(Resolution aResolution);
    static int GetWidth(Resolution aResolution, AspectRatio anAspectRatio);
    static bool IsAspectRatioDisplayable(Resolution aResolution, AspectRatio anAspectRatio, int aMaxTextureSize);

    void DrawAspectRatioRadioButton(const char* aLabel, AspectRatio aCase, int aMaxTextureSize);
    void DrawTakeScreenshot(int aMaxTextureSize);

    Strategy myStrategy {Strategy::Preset};
    Resolution myResolution {Resolution::R_4K};
    AspectRatio myAspectRatio {AspectRatio::A_16_9};
    std::array<int, 2> myCustomResolution {800, 800};
    bool myIsActive {false};
    bool myShouldTakeScreenshot {false};
};
