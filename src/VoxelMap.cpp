#include "Logger.h"
#include "VoxelMap.h"

VoxelMap::VoxelMap()
{
}

bool VoxelMap::Initialize(ImageManager& imageManager, ShaderManager& shaderManager)
{
    Logger::Log("Voxel texture loading...");
    GLuint textureId = imageManager.AddImage("images/voxelTextureMap.png");
    if (textureId == 0)
    {
        return false;
    }

    voxelTextures = imageManager.GetImage(textureId);
    Logger::Log("Voxel texture loading successful.");

    // Voxel Map shader creation. Note we also get the location of the matrix and texturee to set later.
    Logger::Log("Voxel Map shader creation...");
	if (!shaderManager.CreateShaderProgramWithGeometryShader("voxelMapRender", &voxelMapRenderProgram))
	{
		return false;
	}

    projLocation = glGetUniformLocation(voxelMapRenderProgram, "projMatrix");
    xyLengthsLocation = glGetUniformLocation(voxelMapRenderProgram, "xyLengths");

    textureLocation = glGetUniformLocation(voxelMapRenderProgram, "voxelTextures");
    voxelTopTextureLocation = glGetUniformLocation(voxelMapRenderProgram, "voxelTopTexture");
	Logger::Log("Voxel Map shader creation successful!");

    // OpenGL drawing data.
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

    glGenBuffers(1, &positionBuffer);
	glGenBuffers(1, &colorBuffer);
	glGenBuffers(1, &uvBuffer);

	glGenTextures(1, &voxelTopTexture);

	// Send some test data to OpenGL.
	testVertices.positions.push_back(vmath::vec3(0, 0, 0));
	testVertices.positions.push_back(vmath::vec3(0, 1, 0));
	testVertices.positions.push_back(vmath::vec3(1, 1, 0));

	testVertices.colors.push_back(vmath::vec3(1, 0, 0));
	testVertices.colors.push_back(vmath::vec3(0, 1, 0));
	testVertices.colors.push_back(vmath::vec3(0, 0, 1));

    testVertices.uvs.push_back(vmath::vec2(0, 0));
    testVertices.uvs.push_back(vmath::vec2(1, 0));
    testVertices.uvs.push_back(vmath::vec2(1, 1));

	testVertices.TransferPositionToOpenGl(positionBuffer);
	testVertices.TransferColorToOpenGl(colorBuffer);
	testVertices.TransferUvsToOpenGl(uvBuffer);

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

void VoxelMap::Update()
{
    // TODO update the item we're rendering.
}

void VoxelMap::Render(vmath::mat4& projectionMatrix)
{
    glUseProgram(voxelMapRenderProgram);

    // Bind our textures
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, voxelTextures.textureId);
    glUniform1i(textureLocation, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_1D, voxelTopTexture);
    glUniform1i(voxelTopTextureLocation, 1);

    // Bind our vertex data
    glBindVertexArray(vao);
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, projectionMatrix);
	glUniform2i(xyLengthsLocation, mapInfo->xSize, mapInfo->ySize);

	glDrawArraysInstanced(GL_TRIANGLES, 0, testVertices.positions.size(), mapInfo->GetVoxelCount());
}

VoxelMap::~VoxelMap()
{
    glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &positionBuffer);
	glDeleteBuffers(1, &colorBuffer);
	glDeleteBuffers(1, &uvBuffer);

	glDeleteTextures(1, &voxelTopTexture);
}
