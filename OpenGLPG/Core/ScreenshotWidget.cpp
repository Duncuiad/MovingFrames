#include "OpenGLPG_Base.h"

#include "ScreenshotWidget.h"

#include "Assert.h"
#include "Defines.h"
#include "ImGuiWidgets.h"

void ScreenshotWidget::DrawWidget()
{
    myShouldTakeScreenshot = false;

    if (ImGui::BeginPopupModal("Screenshot Settings", &myIsActive,
                               ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize))
    {
        const int maxTextureSize {GetMaxTextureSize()};

        ImGui::BeginTable("##ScreenshotDataTable", 2, ImGuiTableFlags_SizingStretchSame);

        ImGui::TableNextColumn();
        Widgets::RadioButton("Preset", &myStrategy, Strategy::Preset);
        ImGui::TableNextColumn();
        Widgets::RadioButton("Custom", &myStrategy, Strategy::Custom);

        ImGui::TableNextRow();

        DrawResolutionSelection(maxTextureSize);

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::NewLine();
        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        DrawTakeScreenshot(maxTextureSize);

        ImGui::TableNextColumn();

        if (ImGui::Button("Close", ImVec2(150.f, 0.f)))
        {
            myIsActive = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndTable();
        ImGui::EndPopup();
    }
}

void ScreenshotWidget::DrawResolutionSelection(float aMaxTextureSize)
{
    switch (myStrategy)
    {
    case Strategy::Preset: {
        ImGui::TableNextColumn();

        ImGui::Text("Resolution");
        Widgets::RadioButton("1K", &myResolution, Resolution::R_1K);
        Widgets::RadioButton("2K", &myResolution, Resolution::R_2K);
        Widgets::RadioButton("4K", &myResolution, Resolution::R_4K);

        ImGui::TableNextColumn();

        ImGui::Text("Aspect Ratio");
        DrawAspectRatioRadioButton("21:9", AspectRatio::A_21_9, aMaxTextureSize);
        DrawAspectRatioRadioButton("16:9", AspectRatio::A_16_9, aMaxTextureSize);
        DrawAspectRatioRadioButton("4:3", AspectRatio::A_4_3, aMaxTextureSize);
        DrawAspectRatioRadioButton("1:1", AspectRatio::A_1_1, aMaxTextureSize);
        DrawAspectRatioRadioButton("3:4", AspectRatio::A_3_4, aMaxTextureSize);
        DrawAspectRatioRadioButton("9:16", AspectRatio::A_9_16, aMaxTextureSize);
        DrawAspectRatioRadioButton("9:21", AspectRatio::A_9_21, aMaxTextureSize);
        break;
    }
    case Strategy::Custom: {
        ImGui::TableNextColumn();

        ImGui::Text("Width");
        ImGui::SliderInt("##Width", &myCustomResolution[0], 1, GetMaxWidth(), "%d", ImGuiSliderFlags_AlwaysClamp);

        ImGui::TableNextColumn();

        ImGui::Text("Height");
        ImGui::SliderInt("##Height", &myCustomResolution[1], 1, GetMaxHeight(), "%d", ImGuiSliderFlags_AlwaysClamp);
        break;
    }
    default: {
    }
    }
}

int ScreenshotWidget::GetMaxWidth() const
{
    return 2160 * 21 / 9;
}

int ScreenshotWidget::GetMaxHeight() const
{
    return 2160 * 21 / 9;
}

int ScreenshotWidget::GetWidth() const
{
    switch (myStrategy)
    {
    case Strategy::Preset:
        return GetWidth(myResolution, myAspectRatio);
    case Strategy::Custom:
        return myCustomResolution[0];
    default:
        ASSERT(false, "Unsupported option");
        return -1;
    }
}

int ScreenshotWidget::GetHeight() const
{
    switch (myStrategy)
    {
    case Strategy::Preset:
        return GetHeight(myResolution);
    case Strategy::Custom:
        return myCustomResolution[1];
    default:
        ASSERT(false, "Unsupported option");
        return -1;
    }
}

void ScreenshotWidget::ActivateWidget()
{
    ImGui::OpenPopup("Screenshot Settings");
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    myIsActive = true;
}

bool ScreenshotWidget::ShouldTakeScreenshot() const
{
    return myShouldTakeScreenshot;
}

int ScreenshotWidget::GetMaxTextureSize()
{
    int maxTextureSize {0};
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
    return maxTextureSize;
}

int ScreenshotWidget::GetHeight(Resolution aResolution)
{
    switch (aResolution)
    {
    case Resolution::R_1K: {
        return 1080;
    }
    case Resolution::R_2K: {
        return 1440;
    }
    case Resolution::R_4K: {
        return 2160;
    }
    default:
        break;
    }
    return 0;
}

int ScreenshotWidget::GetWidth(Resolution aResolution, AspectRatio anAspectRatio)
{
    const int height {GetHeight(aResolution)};
    switch (anAspectRatio)
    {
    case AspectRatio::A_21_9: {
        return height * 21 / 9;
    }
    case AspectRatio::A_16_9: {
        return height * 16 / 9;
    }
    case AspectRatio::A_4_3: {
        return height * 4 / 3;
    }
    case AspectRatio::A_1_1: {
        return height;
    }
    case AspectRatio::A_3_4: {
        return height * 3 / 4;
    }
    case AspectRatio::A_9_16: {
        return height * 9 / 16;
    }
    case AspectRatio::A_9_21: {
        return height * 9 / 21;
    }
    default:
        break;
    }
    return 0;
}

bool ScreenshotWidget::IsAspectRatioDisplayable(Resolution aResolution, AspectRatio anAspectRatio, int aMaxTextureSize)
{
    return GetWidth(aResolution, anAspectRatio) <= aMaxTextureSize && GetHeight(aResolution) <= aMaxTextureSize;
}

void ScreenshotWidget::DrawAspectRatioRadioButton(const char* aLabel, AspectRatio aCase, int aMaxTextureSize)
{
    const bool isDisplayable {IsAspectRatioDisplayable(myResolution, aCase, aMaxTextureSize)};

    if (!isDisplayable)
    {
        ImGui::BeginDisabled();
    }

    Widgets::RadioButton(aLabel, &myAspectRatio, aCase);

    if (!isDisplayable)
    {
        ImGui::EndDisabled();
    }
}

void ScreenshotWidget::DrawTakeScreenshot(int aMaxTextureSize)
{
    const bool isDisplayable {IsAspectRatioDisplayable(myResolution, myAspectRatio, aMaxTextureSize)};

    if (!isDisplayable)
    {
        ImGui::BeginDisabled();
    }
    if (ImGui::Button("Take Screenshot", ImVec2(150.f, 0.f)))
    {
        myShouldTakeScreenshot = true;
    }
    if (!isDisplayable)
    {
        ImGui::EndDisabled();
    }
}
