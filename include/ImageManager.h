#pragma once

#include <map>
#include <stb\stb_image.h>
#include <GL\glew.h>
#include "ImageTexture.h"

// Holds *all* of the texture images and communicates using 'stb_image'.
// Note that this only supports PNG images, which can be changed by modifying stb_implementations.cpp
class ImageManager
{
    std::map<GLuint, ImageTexture> imageTextures;

public:
    ImageManager();
    GLuint AddImage(const char* filename);
    const ImageTexture& GetImage(GLuint textureId);

    ~ImageManager();
};

