#include "OpenGLPG_Base.h"

#include "ClientCallbacks.h"

#include <GLFW/glfw3.h>

void ClientCB::FrameBufferSizeCallback(GLFWwindow* aWindow, int aWidth, int aHeight)
{
    glViewport(0, 0, aWidth, aHeight);
}
