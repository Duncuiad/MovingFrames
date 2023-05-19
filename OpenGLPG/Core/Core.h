#pragma once

#include "Client.h"

int main();

bool ClientInit(Client& aContext);
bool GLFWInit(Client& aContext);
bool GLADInit();
void Render(GLFWwindow* aWindow);
void ClientShutdown(Client& aContext);

void FrameBufferSizeCallback(GLFWwindow* aWindow, int aWidth, int aHeight);
void ProcessInput(GLFWwindow* aWindow);
