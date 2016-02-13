#include <string>
#include <sstream>
#include <stb\stb_image.h>
#include "Logger.h"
#include "Scenery.h"

Scenery::Scenery(ModelManager* modelManager)
{
    this->modelManager = modelManager;

    groundOrientation = vec::mat4::identity();
}

bool Scenery::Initialize(ShaderManager& shaderManager)
{
    // Load the ground
    groundModelId = modelManager->LoadModel("models/scenery/ground");
    if (groundModelId == 0)
    {
        Logger::Log("Error loading the ground scenery model!");
        return false;
    }

    // Setup the sky program and sky image.

    // Sky program.
    if (!shaderManager.CreateShaderProgram("skyCubeRender", &skyCubeProgram))
    {
        Logger::Log("Failure creating the sky cube shader program!");
        return false;
    }

    viewMatrixLocation = glGetUniformLocation(skyCubeProgram, "viewMatrix");
    skyCubeMapLocation = glGetUniformLocation(skyCubeProgram, "skyCubeMap");

    // Sky Image
    int width;
    int height;
    if (!GetRawImage("images/scenery/sky.png", &rawImage, &width, &height))
    {
        return false;
    }

    if (height != 6 * width)
    {
        Logger::Log("There are not six square sky images in a vertical row in the skybox image!");
        Logger::Log("[This means that height != 6 * width of the image].");
        return false;
    }

    // The sky image is stored as squares with height = 6 * width. Break and send each apart individually.
    glGenVertexArrays(1, &skyCubeVao);
    glBindVertexArray(skyCubeVao);

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &skyCubeTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyCubeTexture);

    glTexStorage2D(GL_TEXTURE_CUBE_MAP, 1, GL_RGBA, width, width);
    for (int i = 0; i < 6; i++)
    {
        glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 0, 0, width, width, GL_RGBA, GL_UNSIGNED_BYTE,
            rawImage + i * (width * width * 4));
    }

    return true;
}

void Scenery::Render(vec::mat4& viewMatrix, vec::mat4& projectionMatrix)
{
    // Render the ground plane
    modelManager->RenderModel(projectionMatrix, groundModelId, groundOrientation, false);

    // Render the sky
    glUseProgram(skyCubeProgram);
    glBindVertexArray(skyCubeVao);

    glBindTexture(GL_TEXTURE_CUBE_MAP, skyCubeTexture);
    glUniform1i(skyCubeMapLocation, 0);

    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, viewMatrix);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

bool Scenery::GetRawImage(const char* filename, unsigned char** data, int* width, int* height)
{
    // Load in the image
    int channels = 0;
    *data = stbi_load(filename, width, height, &channels, STBI_rgb_alpha);
    if (*data && width && height)
    {
        return true;
    }
    else
    {
        std::stringstream errStream;
        errStream << "Failed to load image:" << stbi_failure_reason();
        Logger::LogError(errStream.str().c_str());
        return false;
    }
}

void Scenery::FreeRawImage(unsigned char* imageData)
{
    stbi_image_free(imageData);
}

Scenery::~Scenery()
{
    glDeleteVertexArrays(1, &skyCubeVao);
    glDeleteTextures(1, &skyCubeTexture);

    FreeRawImage(rawImage);
}
