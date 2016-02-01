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

    // Sends the texture data to OpenGL, creating the texture and sending back that id.
    GLuint CreateTexture(int width, int height, unsigned char* imageData);

public:
    ImageManager();
    GLuint AddImage(const char* filename);
    const ImageTexture& GetImage(GLuint textureId);

    // Loads an empty image (RGB-pattern). NOTE: Requires a call to ResendToOpenGL to be visible after data changes!
    GLuint LoadEmpty(int width, int height);

    // Copies the source image into the destination image, writing it at the given x and y positions.
    //  WARNING: Has no out-of-bounds prevention, only supports source moves that fully fit in the destination. (TODO fix this, return false).
    void CopyToImage(GLuint srcImage, GLuint dstImage, int dstX, int dstY);

    // Resends data to OpenGL. Only useful when image data has changed.
    void ResendToOpenGl(GLuint imageId);

    ~ImageManager();
};

