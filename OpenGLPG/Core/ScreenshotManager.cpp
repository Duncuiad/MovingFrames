#include "OpenGLPG_Base.h"

#include "ScreenshotManager.h"

#include "ImageUtils.h"

namespace ScreenshotManager_Private
{
constexpr const char* theScreenshotRenderPassName {"Screenshot"};
}

ScreenshotManager::ScreenshotManager(const ConstructionParams& someParams)
    : myRenderer {someParams.myRenderer}
    , myLocalScreenshotFolder {someParams.myLocalFolder + "Screenshots/"}
{
    myRenderer.CreateRenderPass(ScreenshotManager_Private::theScreenshotRenderPassName, myWidget.GetMaxWidth(),
                                myWidget.GetMaxHeight());
}

void ScreenshotManager::Update(const UpdateParams& someParams)
{
    const GameInputData::InputContainer& inputs {someParams.myGameInputData.myInputs};
    bool takeScreenshot {false};
    if (inputs.at(GameInput::theTakeScreenshot) > 0.f)
    {
        if (inputs.at(GameInput::theHoldShift) > 0.f)
        {
            myWidget.ActivateWidget();
        }
        else
        {
            takeScreenshot = true;
        }
    }
    myWidget.DrawWidget();
    takeScreenshot |= myWidget.ShouldTakeScreenshot();

    if (takeScreenshot)
    {
        const int width {myWidget.GetWidth()};
        const int height {myWidget.GetHeight()};

        myRenderer.Render({someParams.myWorld,
                           {0.f, 0.f},
                           {static_cast<float>(width), static_cast<float>(height)},
                           ScreenshotManager_Private::theScreenshotRenderPassName});

        const unsigned char* pixelData {
            myRenderer.GetRenderedTexture(ScreenshotManager_Private::theScreenshotRenderPassName)};

        SaveImage(pixelData, width, height, 3, myLocalScreenshotFolder);
    }
}
