#pragma once
#include <GL/glew.h>
#include <map>
#include <string>
#include <vector>
#include "ImageManager.h"
#include "ShaderManager.h"
#include "Model.h"
#include "vmath.hpp"

struct PosUvPair
{
    unsigned int positionId;
    unsigned int uvId;
};

// Assists with loading in 3D models
class ModelManager
{
    public:
        // Clears the next model ID and initializes the local reference to the image manager.
        ModelManager(ImageManager* imageManager);

        // Loads a new textured OBJ model, returning the model ID. Returns 0 on failure.
        unsigned int LoadModel(const char* rootFilename);

        // Retrieves a 3D model, returning the model ID.
        const TextureModel& GetModel(unsigned int id);

        unsigned int GetCurrentModelCount() const;

        // Renders the specified model given by the ID.
        void RenderModel(vmath::mat4& projectionMatrix, unsigned int id, vmath::mat4& mvMatrix);

        // Initializes the OpenGL resources
        bool InitializeOpenGlResources(ShaderManager& shaderManager);

        // Sends in the model data to OpenGL.
        void ResetOpenGlModelData();

        // Deletes all initialized OpenGL resources.
        ~ModelManager();

    private:
        ImageManager* imageManager;

        // Rendering data
        GLuint vao;
        GLuint uvBuffer;
        GLuint positionBuffer;
        GLuint indexBuffer;
        GLuint modelRenderProgram;
        GLuint textureLocation;
        GLuint mvLocation;
        GLuint projLocation;

        // Model data
        unsigned int nextModelId;
        std::map<unsigned int, TextureModel> models;

        // Temporary loading structures.
        std::vector<vmath::vec2> rawUvs;
        std::vector<PosUvPair> rawIndices;
        std::map<unsigned int, std::vector<PosUvPair>> uvVertexRemapping;

        // Given a position index and UV coordinate index, returns the actual index to use.
        unsigned int GetActualVertexIndex(unsigned int positionIdx, unsigned int uvIdx, universalVertices& vertices);

        // Parses an individual line of an OBJ model file.
        bool ParseLine(const std::vector<std::string>& line, universalVertices& vertices);

        // Loads an OBJ model into the specified vertices, returning true on success.
        // Note that the OBJ model must fully specify all positions / UVs *before* any indices.
        bool LoadModel(const char* objFilename, universalVertices& vertices, vmath::vec3* minBounds, vmath::vec3* maxBounds);
};
