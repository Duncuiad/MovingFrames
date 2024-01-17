#pragma once

#include "Filepath.h"

bool SaveImage(unsigned char const* someData, int aWidth, int aHeight, int aChannelCount, const Filepath& aFolder,
               const Filepath& aName = "");
bool SaveImage(unsigned int const* someData, int aWidth, int aHeight, int aChannelCount, const Filepath& aFolder,
               const Filepath& aName = "");
bool SaveImage(float const* someData, int aWidth, int aHeight, int aChannelCount, const Filepath& aFolder,
               const Filepath& aName = "");
bool SaveImageHDR(float const* someData, int aWidth, int aHeight, const Filepath& aFolder, const Filepath& aName = "");
