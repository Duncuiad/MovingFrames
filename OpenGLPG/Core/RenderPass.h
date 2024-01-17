#pragma once

#include "Defines.h"

class RenderPass
{
public:
    RenderPass(int aMaxWidth, int aMaxHeight);

    void GetSize(int& aWidthOut, int& aHeightOut) const;
    unsigned char* GetPixelData() const;
    bool Resize(int aWidth, int aHeight);
    void Bind() const;
    void Unbind();
    void Clear();
    void Shutdown();

private:
    const int myMaxWidth;
    const int myMaxHeight;
    int myWidth {0};
    int myHeight {0};
    mutable int myCachedWidth {0};
    mutable int myCachedHeight {0};
    unsigned char* myPixelData {nullptr};

    GLUID myFrameBufferObject {0u};
    GLUID myRenderBufferObject {0u};
    GLUID myRenderTexture {0u};
};
