#pragma once
#include <GL\glew.h>
#include "ModelManager.h"
#include "ShaderManager.h"
#include "Vec.h"

// Holds static scenery objects that are rendered.
class Scenery
{
    public:
        Scenery(ModelManager* modelManager);

        bool Initialize(ShaderManager& shaderManager);
        void Render(vec::mat4& viewMatrix, vec::mat4& projectionMatrix);

        ~Scenery();

    protected:
    private:
        unsigned char* rawImage;
        bool GetRawImage(const char* filename, unsigned char** data, int* width, int* height);
        void FreeRawImage(unsigned char* imageData);

        // Ground plane
        vec::mat4 groundOrientation;
        unsigned int groundModelId;

        // Sky cube
        GLuint skyCubeProgram;
        GLuint viewMatrixLocation;
        GLuint skyCubeMapLocation;

        GLuint skyCubeVao;
        GLuint skyCubeTexture;

        ModelManager* modelManager;
};

