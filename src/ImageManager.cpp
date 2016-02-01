#include <cstring>
#include <string>
#include <sstream>
#include <GL\glew.h>
#include "Logger.h"
#include "ImageManager.h"

ImageManager::ImageManager()
{
}

// Gets additional image data given the texture ID.
const ImageTexture& ImageManager::GetImage(GLuint textureId)
{
    return imageTextures[textureId];
}

// Adds an image to the list of tracked images, returning the texture ID of that image.
GLuint ImageManager::AddImage(const char* filename)
{
    // Load in the image
    int width = 0;
    int height = 0;
    int channels = 0;
    unsigned char* imageData = stbi_load(filename, &width, &height, &channels, STBI_rgb_alpha);
    if (imageData && width && height)
    {
        return CreateTexture(width, height, imageData);
    }
    else
    {
        std::stringstream errStream;
        errStream << "Failed to load image:" << stbi_failure_reason();
        Logger::LogError(errStream.str().c_str());
    }

    return 0;
}

GLuint ImageManager::LoadEmpty(int width, int height)
{
    unsigned char* data = new unsigned char[width * height * 4];
    for (unsigned int i = 0; i < width * height; i++)
    {
        unsigned char redFactor = (i % 3 == 0) ? 255 : 0;
        unsigned char greenFactor = (i % 3 == 1) ? 255 : 0;
        unsigned char blueFactor = (i % 3 == 2) ? 255 : 0;

        data[i * 4] = redFactor;
        data[i * 4 + 1] = greenFactor;
        data[i * 4 + 2] = blueFactor;
        data[i * 4 + 3] = 255;
    }

    GLuint imageId = CreateTexture(width, height, data);
    imageTextures[imageId].loadedFromStb = false;

    return imageId;
}

void ImageManager::CopyToImage(GLuint srcImage, GLuint dstImage, int dstX, int dstY)
{
    // We have to copy over data in scanlines because the destination image is likely a different size.
    int scanlineSize = imageTextures[srcImage].width * 4;
    for (int i = 0; i < imageTextures[srcImage].height; i++)
    {
        const unsigned char* startingLocation = imageTextures[srcImage].imageData + i * scanlineSize;
        unsigned char* destinationLocation = imageTextures[dstImage].imageData + (dstX * 4) + (dstY + i) * 4 * imageTextures[dstImage].width;
        memcpy(destinationLocation, startingLocation, sizeof(unsigned char) * scanlineSize);
    }
}

GLuint ImageManager::CreateTexture(int width, int height, unsigned char* imageData)
{
    // Create a new texture for the image.
    GLuint newTextureId;
    glGenTextures(1, &newTextureId);

    imageTextures[newTextureId] = ImageTexture(newTextureId, imageData, width, height);

    // Bind the texture and send in image data
    glBindTexture(GL_TEXTURE_2D, newTextureId);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
    ResendToOpenGl(newTextureId);

    return newTextureId;
}

void ImageManager::ResendToOpenGl(GLuint imageId)
{
    const ImageTexture& image = imageTextures[imageId];
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image.width, image.height, GL_RGBA, GL_UNSIGNED_BYTE, image.imageData);
}

ImageManager::~ImageManager()
{
    // Free all of the loaded textures and in-memory image data at program end.
    for (std::map<GLuint, ImageTexture>::iterator iterator = imageTextures.begin(); iterator != imageTextures.end(); iterator++)
    {
        glDeleteTextures(1, &iterator->first);

        if (iterator->second.loadedFromStb)
        {
            stbi_image_free(iterator->second.imageData);
        }
        else
        {
            delete[] iterator->second.imageData;
        }
    }
}
