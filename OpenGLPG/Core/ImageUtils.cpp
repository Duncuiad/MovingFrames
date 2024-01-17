#include "OpenGLPG_Base.h"

#include "ImageUtils.h"

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#ifndef __STDC_LIB_EXT1__
#define __STDC_LIB_EXT1__
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#undef __STDC_LIB_EXT1__
#endif

namespace ImageUtils_Private
{
Filepath ProcessPath(const Filepath& aFolder, const Filepath& aName, const char* anExtension)
{
    Filepath name {aName};
    if (name == "")
    {
        time_t t = std::time(nullptr);
        tm localtime;
        localtime_s(&localtime, &t);
        std::ostringstream stream;
        stream << std::put_time(&localtime, "%d-%m-%y_%H-%M-%S");
        name = stream.str();
    }
    else if (!name.HasExtension(anExtension))
    {
        name.RemoveExtension();
        std::cerr << "Requested saving image with wrong file extension. Overriding with ." << anExtension << std::endl;
    }
    if (!name.HasExtension())
    {
        name = name + "." + anExtension;
    }
    return aFolder + name;
}
} // namespace ImageUtils_Private

bool SaveImage(unsigned char const* someData, int aWidth, int aHeight, int aChannelCount, const Filepath& aFolder,
               const Filepath& aName)
{
    stbi_flip_vertically_on_write(true);
    Filepath path = ImageUtils_Private::ProcessPath(aFolder, aName, "png");
    return stbi_write_png(path.GetBuffer(), aWidth, aHeight, aChannelCount, someData, aChannelCount * aWidth) != 0;
}

bool SaveImage(unsigned int const* someData, int aWidth, int aHeight, int aChannelCount, const Filepath& aFolder,
               const Filepath& aName)
{
    stbi_flip_vertically_on_write(true);
    Filepath path = ImageUtils_Private::ProcessPath(aFolder, aName, "png");
    return stbi_write_png(path.GetBuffer(), aWidth, aHeight, aChannelCount, someData, aChannelCount * aWidth * 4);
}

bool SaveImage(float const* someData, int aWidth, int aHeight, int aChannelCount, const Filepath& aFolder,
               const Filepath& aName)
{
    stbi_flip_vertically_on_write(true);
    unsigned char* data = new unsigned char[aWidth * aHeight * aChannelCount];
    for (int i = 0; i < aWidth * aHeight * aChannelCount; ++i)
    {
        data[i] = static_cast<unsigned char>(someData[i] * 255.f);
    }
    const bool result = SaveImage(data, aWidth, aHeight, aChannelCount, aFolder, aName);
    delete[] data;
    return result;
}

bool SaveImageHDR(float const* someData, int aWidth, int aHeight, const Filepath& aFolder, const Filepath& aName)
{
    stbi_flip_vertically_on_write(true);
    Filepath path = ImageUtils_Private::ProcessPath(aFolder, aName, "hdr");
    return stbi_write_hdr(path.GetBuffer(), aWidth, aHeight, STBI_rgb, someData) != 0;
}
