#pragma once
#include <GL\glew.h>
#include "Vertex.h"

// Holds data from a loaded, indexed UV textured model.
struct TextureModel
{
    // Texture to apply to the model.
    GLuint textureId;

    // Vertex data -- only the position, uvs, and indices fields are expected to be filled.
    universalVertices vertices;
};
