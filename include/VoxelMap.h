#pragma once
#include <GL\glew.h>
#include <vector>
#include "ImageManager.h"
#include "MapInfo.h"
#include "ModelManager.h"
#include "ShaderManager.h"
#include "Vertex.h"
#include "Vec.h"

// The game map of voxels.
class VoxelMap
{
    public:
        VoxelMap();

        // Sets up the voxel map OpenGL data and compiles the associated shader. Note we only use the model manager to get at UV data easily.
        bool Initialize(ImageManager& imageManager, ModelManager& modelManager, ShaderManager& shaderManager);

        // Sets up the VoxelMap from the provided map info.
        void SetupFromMap(const MapInfo& mapInfo);

        // Sets the currently-selected voxel, which renders specially.
        void SetSelectedVoxel(const vec::vec3i& selectedVoxel);

        // Renders the voxel map, using the current viewer position matrix.
        void Render(const vec::mat4& projectionMatrix);

        // Deletes any OpenGL voxel resources that have been consumed.
        ~VoxelMap();

    private:
        bool CreateVoxelShader(ShaderManager& shaderManager);
        void InitOpenGl();

        // Loads voxel textures, returning the ImageManager ID of the textures.
        std::vector<GLuint> LoadVoxelTextures(ImageManager& imageManager);

        // Loads models, returning the ModelManager ID of the models.
        std::vector<int> LoadModels(ModelManager& modelManager);

        // Minimal map data required for rendering. The rest of the data is stored in the texture.
        bool hasValidMap;
        int xMapSize;
        int yMapSize;
        int totalVoxelSize;

        // The textures for all of the voxels in a single nicely-packed image.
        GLuint voxelTextureId;

        // The Voxel rendering program and locations of textures we need within it.
        GLuint voxelMapRenderProgram;
        GLuint projLocation;
        GLuint xyLengthsLocation;
        GLuint currentVoxelIdLocation;

        GLuint selectedIndexLocation;
        vec::vec3i selectedVoxel;

        GLuint textureLocation;
        GLuint voxelTopTextureLocation;

        // Actual texture for voxel type, orientation, and property
        GLuint voxelTopTexture;

        // Vertices and associated OpenGL functionality
        universalVertices voxelVertices;
        std::vector<int> voxelIndexOffsets;
        std::vector<int> voxelIndexCounts;

        GLuint vao;
        GLuint positionBuffer;
        GLuint indexBuffer;
        GLuint uvBuffer;
};
