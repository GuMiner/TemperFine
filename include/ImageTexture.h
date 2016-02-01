#pragma once
#include <GL\glew.h>

// Holds a pointer to image texture data in GL_RGBA32F fomat.
struct ImageTexture
{
    GLuint textureId;

    int width;
    int height;

    // Image data as loaded in.
    unsigned char* imageData;

    // True if loaded from STB (file), false otherwise.
    bool loadedFromStb;

    ImageTexture()
    {
    }

    ImageTexture(GLuint textureId, unsigned char* imageData, int width, int height)
        : textureId(textureId), width(width), height(height), imageData(imageData), loadedFromStb(true)
    {
    }
};
