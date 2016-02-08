#pragma once
#include <GL\glew.h>
#include "ModelManager.h"
#include "ShaderManager.h"
#include "vmath.hpp"

// Holds static scenery objects that are rendered.
class Scenery
{
    public:
        Scenery(ModelManager* modelManager);

        bool Initialize(ShaderManager& shaderManager);
        void Render(vmath::mat4& viewMatrix, vmath::mat4& projectionMatrix);

        ~Scenery();

    protected:
    private:
        unsigned char* rawImage;
        bool GetRawImage(const char* filename, unsigned char** data, int* width, int* height);
        void FreeRawImage(unsigned char* imageData);

        // Ground plane
        vmath::mat4 groundOrientation;
        unsigned int groundModelId;

        // Sky cube
        GLuint skyCubeProgram;
        GLuint viewMatrixLocation;
        GLuint skyCubeMapLocation;

        GLuint skyCubeVao;
        GLuint skyCubeTexture;

        ModelManager* modelManager;
};

