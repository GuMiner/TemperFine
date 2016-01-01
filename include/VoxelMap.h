#pragma once
#include <GL\glew.h>
#include "ShaderManager.h"
#include "Vertex.h"
#include "vmath.hpp"

// The game map of voxels.
class VoxelMap
{
    public:
        VoxelMap();

        // Sets up the voxel map OpenGL data and compiles the associated shader.
        bool Initialize(ShaderManager& shaderManager);

        // Updates the voxel map
        void Update();

        // Renders the voxel map, using the current viewer position matrix.
        void Render(vmath::mat4& projectionMatrix);

        // Deletes any OpenGL voxel resources that have been consumed.
        ~VoxelMap();
    protected:
    private:
        GLuint voxelMapRenderProgram;
        GLuint projLocation;

        universalVertices testVertices;

        GLuint vao;
        GLuint positionBuffer;
        GLuint colorBuffer;
};
