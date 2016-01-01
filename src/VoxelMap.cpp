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

    // Voxel Map shader creation. Note we also get the location of the matrix to set later.
    Logger::Log("Voxel Map shader creation...");
	if (!shaderManager.CreateShaderProgram("voxelMapRender", &voxelMapRenderProgram))
	{
		return false;
	}

    projLocation = glGetUniformLocation(voxelMapRenderProgram, "projMatrix");
	Logger::Log("Voxel Map shader creation successful!");

    // OpenGL drawing data.
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

    glGenBuffers(1, &positionBuffer);
	glGenBuffers(1, &colorBuffer);

	// Send some test data to OpenGL
	testVertices.positions.push_back(vmath::vec3(0, 0, 0));
	testVertices.positions.push_back(vmath::vec3(0, 5, 0));
	testVertices.positions.push_back(vmath::vec3(5, 5, 0));

	testVertices.colors.push_back(vmath::vec3(1, 0, 0));
	testVertices.colors.push_back(vmath::vec3(0, 1, 0));
	testVertices.colors.push_back(vmath::vec3(0, 0, 1));

	testVertices.TransferPositionToOpenGl(positionBuffer);
	testVertices.TransferColorToOpenGl(colorBuffer);

	return true;
}

void VoxelMap::Update()
{
    // TODO update the item we're rendering.
}

void VoxelMap::Render(vmath::mat4& projectionMatrix)
{
    glUseProgram(voxelMapRenderProgram);
	glBindVertexArray(vao);

	glUniformMatrix4fv(projLocation, 1, GL_FALSE, projectionMatrix);
	glDrawArrays(GL_TRIANGLES, 0, testVertices.positions.size());
}

VoxelMap::~VoxelMap()
{
    glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &positionBuffer);
	glDeleteBuffers(1, &colorBuffer);
}
