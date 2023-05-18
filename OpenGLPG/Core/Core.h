#pragma once

// GLAD needs to be included first
#include "glad/glad.h"
//

#include "Defines.h"
#include "GLFW/glfw3.h"

int main();

void FrameBufferSizeCallback(GLFWwindow* aWindow, int aWidth, int aHeight);
void ProcessInput(GLFWwindow* aWindow);
void Render(GLFWwindow* aWindow);
