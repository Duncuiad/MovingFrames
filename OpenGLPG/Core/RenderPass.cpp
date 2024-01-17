#include "OpenGLPG_Base.h"

#include "RenderPass.h"

RenderPass::RenderPass(int aMaxWidth, int aMaxHeight)
    : myMaxWidth {aMaxWidth}
    , myMaxHeight {aMaxHeight}
{
    glGenFramebuffers(1, &myFrameBufferObject);
    glGenRenderbuffers(1, &myRenderBufferObject);
    glGenTextures(1, &myRenderTexture);
    glBindFramebuffer(GL_FRAMEBUFFER, myFrameBufferObject);
    glBindRenderbuffer(GL_RENDERBUFFER, myRenderBufferObject);
    glBindTexture(GL_TEXTURE_2D, myRenderTexture);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, myMaxWidth, myMaxHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, myRenderBufferObject);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, myMaxWidth, myMaxHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, myRenderTexture, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass::GetSize(int& aWidthOut, int& aHeightOut) const
{
    aWidthOut = myWidth;
    aHeightOut = myHeight;
}

unsigned char* RenderPass::GetPixelData() const
{
    return myPixelData;
}

bool RenderPass::Resize(int aWidth, int aHeight)
{
    if (aWidth > myMaxWidth || aHeight > myMaxHeight)
    {
        return false;
    }
    myWidth = aWidth;
    myHeight = aHeight;
    return true;
}

void RenderPass::Bind() const
{
    int viewportData[4];
    glGetIntegerv(GL_VIEWPORT, viewportData);
    if (viewportData[2] != myWidth || viewportData[3] != myHeight)
    {
        myCachedWidth = viewportData[2];
        myCachedHeight = viewportData[3];
        glViewport(0, 0, myWidth, myHeight);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, myFrameBufferObject);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderPass::Unbind()
{
    myPixelData = new unsigned char[myWidth * myHeight * 3];

    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glReadPixels(0, 0, myWidth, myHeight, GL_RGB, GL_UNSIGNED_BYTE, myPixelData);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if (myCachedWidth != 0 || myCachedHeight != 0)
    {
        glViewport(0, 0, myCachedWidth, myCachedHeight);
        myCachedWidth = 0;
        myCachedHeight = 0;
    }
}

void RenderPass::Clear()
{
    if (myPixelData != nullptr)
    {
        delete[] myPixelData;
        myPixelData = nullptr;
    }
}

void RenderPass::Shutdown()
{
    glDeleteFramebuffers(1, &myFrameBufferObject);
    glDeleteRenderbuffers(1, &myRenderBufferObject);
    glDeleteTextures(1, &myRenderTexture);
}
