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
    myRenderer.CreateRenderPass(ScreenshotManager_Private::theScreenshotRenderPassName, myWidth, myHeight);
}

void ScreenshotManager::Update(const UpdateParams& someParams)
{
    if (someParams.myGameInputData.myInputs.at(GameInput::theTakeScreenshot) > 0.f)
    {
        myRenderer.Render({someParams.myWorld,
                           {0.f, 0.f},
                           {static_cast<float>(myWidth), static_cast<float>(myHeight)},
                           ScreenshotManager_Private::theScreenshotRenderPassName});

        const unsigned char* pixelData {
            myRenderer.GetRenderedTexture(ScreenshotManager_Private::theScreenshotRenderPassName)};

        SaveImage(pixelData, myWidth, myHeight, 3, myLocalScreenshotFolder);
    }
}
