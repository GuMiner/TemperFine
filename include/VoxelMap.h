#pragma once
#include <GL\glew.h>
#include "ImageManager.h"
#include "MapInfo.h"
#include "ShaderManager.h"
#include "Vertex.h"
#include "vmath.hpp"

// The game map of voxels.
class VoxelMap
{
    public:
        VoxelMap();

        // Sets up the voxel map OpenGL data and compiles the associated shader.
        bool Initialize(ImageManager& imageManager, ShaderManager& shaderManager);

        // Sets up the VoxelMap from the provided map info.
        void SetupFromMap(MapInfo* mapInfo);

        // Updates the voxel map
        void Update();

        // Renders the voxel map, using the current viewer position matrix.
        void Render(vmath::mat4& projectionMatrix);

        // Deletes any OpenGL voxel resources that have been consumed.
        ~VoxelMap();
    protected:
    private:
        // The map for which the voxel data is currently created from.
        MapInfo *mapInfo;

        // The textures for all of the voxels in a single nicely-packed image.
        ImageTexture voxelTextures;

        // The Voxel rendering program and locations of textures we need within it.
        GLuint voxelMapRenderProgram;
        GLuint projLocation;
        GLuint xyLengthsLocation;

        GLuint textureLocation;
        GLuint voxelTopTextureLocation;

        // Actual texture for voxel type, orientation, and property
        GLuint voxelTopTexture;

        // Vertices and associated OpenGL functionality for the vertices.
        universalVertices testVertices;

        GLuint vao;
        GLuint positionBuffer;
        GLuint colorBuffer;
        GLuint uvBuffer;
};
