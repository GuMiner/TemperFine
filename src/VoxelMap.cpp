#include <sstream>
#include "GraphicsConfig.h"
#include "Logger.h"
#include "VoxelMap.h"

VoxelMap::VoxelMap()
{
}

void VoxelMap::InitOpenGl()
{
    // OpenGL drawing data.
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

    glGenBuffers(1, &positionBuffer);
	glGenBuffers(1, &indexBuffer);
	glGenBuffers(1, &uvBuffer);

	glGenTextures(1, &voxelTopTexture);
}

bool VoxelMap::CreateVoxelShader(ShaderManager& shaderManager)
{
    // Voxel Map shader creation. Note we also get the location of the matrix and textures to set later.
    Logger::Log("Voxel Map shader creation...");
	if (!shaderManager.CreateShaderProgramWithGeometryShader("voxelMapRender", &voxelMapRenderProgram))
	{
		return false;
	}

    projLocation = glGetUniformLocation(voxelMapRenderProgram, "projMatrix");
    xyLengthsLocation = glGetUniformLocation(voxelMapRenderProgram, "xyLengths");
    currentVoxelIdLocation = glGetUniformLocation(voxelMapRenderProgram, "currentVoxelId");

    textureLocation = glGetUniformLocation(voxelMapRenderProgram, "voxelTextures");
    voxelTopTextureLocation = glGetUniformLocation(voxelMapRenderProgram, "voxelTopTexture");
	Logger::Log("Voxel Map shader creation successful!");
	return true;
}

std::vector<GLuint> VoxelMap::LoadVoxelTextures(ImageManager& imageManager)
{
    Logger::Log("Voxel texture loading...");

    std::vector<GLuint> voxelTextureIds;
    for (int i = 0; i < GraphicsConfig::VoxelTypes; i++)
    {
        std::stringstream voxelModelTextureName;
        voxelModelTextureName << "models/voxels/voxel_" << i << ".png";

        GLuint textureId = imageManager.AddImage(voxelModelTextureName.str().c_str());
        if (textureId == 0)
        {
            return std::vector<GLuint>();
        }
        voxelTextureIds.push_back(textureId);
    }

    Logger::Log("Voxel texture loading successful.");
    return voxelTextureIds;
}

std::vector<int> VoxelMap::LoadModels(ModelManager& modelManager)
{
    Logger::Log("Voxel model loading...");

    std::vector<int> voxelModelIds;
    for (int i = 0; i < GraphicsConfig::VoxelTypes; i++)
    {
        std::stringstream voxelModelTextureName;
        voxelModelTextureName << "models/voxels/voxel_" << i;

        int modelId = modelManager.LoadModel(voxelModelTextureName.str().c_str());
        if (modelId == 0)
        {
            return std::vector<int>();
        }
        voxelModelIds.push_back(modelId);
    }

    Logger::Log("Voxel model loading successful.");
    return voxelModelIds;
}

bool VoxelMap::Initialize(ImageManager& imageManager, ModelManager& modelManager, ShaderManager& shaderManager)
{
    // Shaders, raw model/image data
    if (!CreateVoxelShader(shaderManager))
    {
        return false; // Bad shader!
    }

    std::vector<GLuint> voxelTextureIds = LoadVoxelTextures(imageManager);
    if (voxelTextureIds.size() == 0)
    {
        return false; // No textures loaded!
    }

    std::vector<int> voxelModelIds = LoadModels(modelManager);
    if (voxelModelIds.size() == 0)
    {
        return false; // No models loaded!
    }

    // At this point, we need to combine the texture images into one larger image (and scale UVs accordingly)
    //  and combine vertex data into one universal array (and scale indices appropriately).
    Logger::Log("Combining voxel images and models into a composite structure...");

    // Combine together the images. Images *must* all be the same size squares.
    int edgeLength = imageManager.GetImage(voxelTextureIds[0]).width;
    int width = GraphicsConfig::VoxelsPerRow * edgeLength;
    int height = (int)voxelTextureIds.size() / GraphicsConfig::VoxelsPerRow;
    if (height == 0 || height * GraphicsConfig::VoxelsPerRow < (int)voxelTextureIds.size())
    {
        ++height;
    }

    height *= edgeLength;
    const ImageTexture& voxelTexture = imageManager.GetImage(imageManager.LoadEmpty(width, height));
    voxelTextureId = voxelTexture.textureId;

    // Store the UV offsets so we can modify them later below for proper texture mapping.
    std::vector<vec::vec2> newUvMin;
    std::vector<vec::vec2> newUvMax;

    int xP = 0;
    int yP = 0;
    for (unsigned int i = 0; i < voxelTextureIds.size(); i++)
    {
        imageManager.CopyToImage(voxelTextureIds[i], voxelTexture.textureId, xP * edgeLength, yP * edgeLength);
        newUvMin.push_back(vec::vec2((float)(xP * edgeLength) / (float)width, (float)(yP * edgeLength) / (float) height));
        newUvMax.push_back(vec::vec2((float)((xP + 1) * edgeLength) / (float)width, (float)((yP + 1) * edgeLength) / (float) height));

        xP++;
        if (xP == GraphicsConfig::VoxelsPerRow)
        {
            xP = 0;
            yP++;
        }
    }

    // Combine together the vertex data.
    unsigned int indexPositionReferralOffset = 0;
    unsigned int indexOffset = 0;
    for (unsigned int i = 0; i < voxelModelIds.size(); i++)
    {
        const TextureModel& model = modelManager.GetModel(voxelModelIds[i]);
        voxelIndexOffsets.push_back(indexOffset);
        voxelIndexCounts.push_back(model.vertices.indices.size());

        voxelVertices.positions.insert(voxelVertices.positions.end(), model.vertices.positions.begin(), model.vertices.positions.end());

        // Update the UVs appropriately, with a minor bit of vector math.
        for (unsigned int j = 0; j < model.vertices.uvs.size(); j++)
        {
            vec::vec2 newUv = newUvMin[i] + model.vertices.uvs[j] * (newUvMax[i] - newUvMin[i]);
            voxelVertices.uvs.push_back(newUv);
        }

        // Update the indices appropriately
        for (unsigned int j = 0; j < model.vertices.indices.size(); j++)
        {
            voxelVertices.indices.push_back(model.vertices.indices[j] + indexPositionReferralOffset);
        }

        indexPositionReferralOffset += model.vertices.positions.size();
        indexOffset += model.vertices.indices.size();
    }

    Logger::Log("Combination complete!");

    // Send our combined data to OpenGL.
    imageManager.ResendToOpenGl(voxelTextureId);
    InitOpenGl();

	voxelVertices.TransferPositionToOpenGl(positionBuffer);
	voxelVertices.TransferUvsToOpenGl(uvBuffer);
	voxelVertices.TransferIndicesToOpenGl(indexBuffer);

	return true;
}

void VoxelMap::SetupFromMap(MapInfo* mapInfo)
{
    this->mapInfo = mapInfo;

    Logger::Log("Creating 1D texture for voxel type, orientation, and property data.");
    int textureLength = mapInfo->GetVoxelCount();
    glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_1D, voxelTopTexture);
	glTexStorage1D(GL_TEXTURE_1D, 1, GL_RGBA8, textureLength);

	Logger::Log("Interlacing the map data...");
	unsigned char* interlacedData = new unsigned char[textureLength * 4];

	for (int i = 0; i < textureLength; i++)
    {
        interlacedData[i * 4] = mapInfo->blockType[i];
        interlacedData[i * 4 + 1] = mapInfo->blockOrientation[i];
        interlacedData[i * 4 + 2] = mapInfo->blockProperty[i];
        interlacedData[i * 4 + 3] = 128; // Some data can go here as it is an RGBA 8-bit image.
    }

	Logger::Log("Sending the data to OpenGL...");
	glTexSubImage1D(GL_TEXTURE_1D, 0, 0, textureLength, GL_RGBA, GL_UNSIGNED_BYTE, &interlacedData[0]);

	delete[] interlacedData;
}

void VoxelMap::Render(vec::mat4& projectionMatrix)
{
    glUseProgram(voxelMapRenderProgram);

    // Bind our textures
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, voxelTextureId);
    glUniform1i(textureLocation, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_1D, voxelTopTexture);
    glUniform1i(voxelTopTextureLocation, 1);

    // Bind our vertex data
    glBindVertexArray(vao);
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, projectionMatrix);

	glUniform2i(xyLengthsLocation, mapInfo->xSize, mapInfo->ySize);

    for (unsigned int i = 0; i < (unsigned int)GraphicsConfig::VoxelTypes; i++)
    {
        glUniform1ui(currentVoxelIdLocation, i + 1);
        glDrawElementsInstanced(GL_TRIANGLES, voxelIndexCounts[i], GL_UNSIGNED_INT, (const void*)(voxelIndexOffsets[i] * sizeof(GL_UNSIGNED_INT)), mapInfo->GetVoxelCount());
    }
}

VoxelMap::~VoxelMap()
{
    glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &positionBuffer);
	glDeleteBuffers(1, &indexBuffer);
	glDeleteBuffers(1, &uvBuffer);

	glDeleteTextures(1, &voxelTopTexture);
}
