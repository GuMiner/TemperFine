#pragma once
#include <GL\glew.h>
#include "Vertex.h"
#include "vmath.hpp"

// Holds data from a loaded, indexed UV textured model.
struct TextureModel
{
    // Texture to apply to the model.
    GLuint textureId;

    // Vertex data -- only the position, uvs, and indices fields are expected to be filled.
    universalVertices vertices;

    // Offset to apply to all the indicies in this model when sending to OpenGL and rendering.
    GLuint indexOffset;

    // Model bounding box.
    vmath::vec3 minBounds;
    vmath::vec3 maxBounds;
};
