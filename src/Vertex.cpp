#include "Vertex.h"
#include <stddef.h>

template<typename T>
void universalVertices::SendToOpenGl(GLuint buffer, GLuint shaderIdx, GLuint itemCount, const std::vector<T>& data)
{
    glEnableVertexAttribArray(shaderIdx);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glVertexAttribPointer(shaderIdx, itemCount, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(T), &data[0], GL_DYNAMIC_DRAW);
}

void universalVertices::SendUIntToOpenGl(GLuint buffer, GLuint shaderIdx, GLuint itemCount, const std::vector<unsigned int>& data)
{
	glEnableVertexAttribArray(shaderIdx);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// Note this is glVertexAttrib*I*Pointer in comparison to the other call above.
	glVertexAttribIPointer(shaderIdx, itemCount, GL_UNSIGNED_INT, 0, nullptr);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(unsigned int), &data[0], GL_DYNAMIC_DRAW);
}

void universalVertices::SendIndicesToOpenGl(GLuint buffer, const std::vector<unsigned int>& data)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(unsigned int), &data[0], GL_DYNAMIC_DRAW);
}

void universalVertices::Reset()
{
	positions.clear();
	colors.clear();
	barycentrics.clear();
	uvs.clear();
	ids.clear();
	indices.clear();
}

void universalVertices::AddColorTextureVertex(vmath::vec3 position, vmath::vec3 color, vmath::vec2 uv)
{
    positions.push_back(position);
    colors.push_back(color);
    uvs.push_back(uv);
}

void universalVertices::TransferPositionToOpenGl(GLuint positionBuffer)
{
	universalVertices::SendToOpenGl(positionBuffer, 0, 3, positions);
}

void universalVertices::TransferColorToOpenGl(GLuint colorBuffer)
{
	universalVertices::SendToOpenGl(colorBuffer, 1, 3, colors);
}

void universalVertices::TransferBarycentricsToOpenGl(GLuint barycentricBuffer)
{
	universalVertices::SendToOpenGl(barycentricBuffer, 2, 4, barycentrics);
}

void universalVertices::TransferUvsToOpenGl(GLuint uvBuffer)
{
	universalVertices::SendToOpenGl(uvBuffer, 3, 2, uvs);
}

void universalVertices::TransferIdsToOpenGl(GLuint idBuffer)
{
	universalVertices::SendUIntToOpenGl(idBuffer, 4, 1, ids);
}

void universalVertices::TransferIndicesToOpenGl(GLuint indiciesBuffer)
{
	universalVertices::SendIndicesToOpenGl(indiciesBuffer, indices);
}

void universalVertices::TransferDirectToOpenGl(const std::vector<vmath::vec4>& positions, GLuint positionBuffer, const std::vector<vmath::vec3>& colors, GLuint colorBuffer)
{
	universalVertices::SendToOpenGl(positionBuffer, 0, 4, positions);
	universalVertices::SendToOpenGl(colorBuffer, 1, 3, colors);

}

void DrawArraysIndirectCommand::Set(GLuint vertexCount, GLuint instanceCount, GLuint firstVertexOffset, GLuint baseInstance)
{
    this->vertexCount = vertexCount;
    this->instanceCount = instanceCount;
    this->firstVertexOffset = firstVertexOffset;
    this->baseInstance = baseInstance;
}