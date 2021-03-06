#include <cstring>
#include <limits>
#include <sstream>
#include "Logger.h"
#include "ModelManager.h"
#include "StringUtils.h"

ModelManager::ModelManager(ImageManager* imageManager)
{
    nextModelId = 1;
    this->imageManager = imageManager;
}

unsigned int ModelManager::GetActualVertexIndex(unsigned int positionIdx, unsigned int uvIdx, universalVertices& vertices)
{
    for(const PosUvPair& pair : uvVertexRemapping[positionIdx])
    {
        if (pair.uvId == uvIdx)
        {
            // Remapping already exists.
            return pair.positionId;
        }
    }

    // Remappings exist, but the UV coordinate listed isn't there. Add a new remapping.
    PosUvPair newVertexRemapping;
    newVertexRemapping.uvId = uvIdx;
    newVertexRemapping.positionId = vertices.positions.size();
    uvVertexRemapping[positionIdx].push_back(newVertexRemapping);

    // Duplicate the vertex -- but with the new UV coordinate
    const PosUvPair& validMapping = uvVertexRemapping[positionIdx][0];
    vertices.positions.push_back(vertices.positions[validMapping.positionId]);
    vertices.uvs.push_back(rawUvs[uvIdx]);

    return newVertexRemapping.positionId;
}

bool ModelManager::ParseLine(const std::vector<std::string>& line, universalVertices& vertices)
{
    // Skip tiny lines.
    if (line.size() < 3)
    {
        return true;
    }

    if (strncmp(line[0].c_str(), "v", 1) == 0 && line[0].size() == 1)
    {
        // Positions
        if (line.size() != 4)
        {
            Logger::Log("Line did not have 3 elements for a vertex position.");
            return false;
        }

        vec::vec3 vector;
        if (!StringUtils::ParseFloatFromString(line[1], vector.x) ||
            !StringUtils::ParseFloatFromString(line[2], vector.y) ||
            !StringUtils::ParseFloatFromString(line[3], vector.z))
        {
            Logger::Log("Line had an improper floating-point value.");
            return false;
        }

        vertices.positions.push_back(vector);
    }
    else if (strncmp(line[0].c_str(), "vt", 2) == 0 && line[0].size() == 2)
    {
        // UV coordinates
        if (line.size() != 3)
        {
            Logger::Log("Line did not have 2 elements for a UV coordinate.");
            return false;
        }

        vec::vec2 vector;
        if (!StringUtils::ParseFloatFromString(line[1], vector.x) ||
            !StringUtils::ParseFloatFromString(line[2], vector.y))
        {
            Logger::Log("Line had an improper floating-point value.");
            return false;
        }

        rawUvs.push_back(vector);
    }
    else if (strncmp(line[0].c_str(), "f", 1) == 0 && line[0].size() == 1)
    {
        // Indices
        if (line.size() != 4)
        {
            Logger::Log("Line did not have 3 elements for an index mapping.");
            return false;
        }

        // Indicies can be processed individually.
        for (unsigned int i = 0; i < 3; i++)
        {
            std::vector<std::string> separatedIndices;
            StringUtils::Split(line[i + 1], '/', true, separatedIndices);
            if (separatedIndices.size() != 2)
            {
                Logger::Log("Line did not have a position and UV coordinate for a portion of an index mapping.");
                return false;
            }

            int positionIndex, uvIndex;
            if (!StringUtils::ParseIntFromString(separatedIndices[0], positionIndex) ||
                !StringUtils::ParseIntFromString(separatedIndices[1], uvIndex))
            {
                Logger::Log("Index had an improper integer value.");
                return false;
            }

            // Indices should be zero-based.
            positionIndex--;
            uvIndex--;

            PosUvPair pair;
            pair.positionId = (unsigned int)positionIndex;
            pair.uvId = (unsigned int)uvIndex;

            rawIndices.push_back(pair);
            indexUvMap[pair.positionId] = pair.uvId;
        }
    }

    return true;
}

bool ModelManager::LoadModel(const char* objFilename, universalVertices& vertices, vec::vec3* minBounds, vec::vec3* maxBounds)
{
    std::string fileString;
    if (!StringUtils::LoadStringFromFile(objFilename, fileString))
    {
        Logger::Log("Could not read the OBJ file!");
        return false;
    }

    std::vector<std::string> fileLines;
    StringUtils::Split(fileString, StringUtils::Newline, true, fileLines);

    rawIndices.clear();
    indexUvMap.clear();
    rawUvs.clear();
    for(const std::string& line : fileLines)
    {
        std::vector<std::string> splitLine;
        StringUtils::Split(line, StringUtils::Space, true, splitLine);
        if (!ParseLine(splitLine, vertices))
        {
            Logger::Log("Error parsing line in OBJ file!");
            Logger::Log(line.c_str());
            return false;
        }
    }

    // Properly render out the vertices so that there is one UV per vertex.
    uvVertexRemapping.clear();

    // Also figure out the min-max bounding box while we're at it.
    minBounds->x = std::numeric_limits<float>::max();
    minBounds->y = std::numeric_limits<float>::max();
    minBounds->z = std::numeric_limits<float>::max();
    maxBounds->x = std::numeric_limits<float>::min();
    maxBounds->y = std::numeric_limits<float>::min();
    maxBounds->z = std::numeric_limits<float>::min();

    // There's guaranteed to be a UV for each point. Find it, and set it.
    for (unsigned int i = 0; i < vertices.positions.size(); i++)
    {
        if (indexUvMap.find(i) != indexUvMap.end())
        {
            PosUvPair pair;
            pair.positionId = i;
            pair.uvId = indexUvMap[i];
            vertices.uvs.push_back(rawUvs[pair.uvId]);

            // Initialize the remapping for this index.
            uvVertexRemapping[i] = std::vector<PosUvPair>();
            uvVertexRemapping[i].push_back(pair);
        }
        else
        {
            std::stringstream errorStream;
            errorStream << "Failed to load in the UV for point " << i << ".";
            Logger::Log(errorStream.str().c_str());
            return false;
        }

        // Perform min-max bounding check.
        if (vertices.positions[i].x < minBounds->x)
        {
            minBounds->x = vertices.positions[i].x;
        }

        if (vertices.positions[i].y < minBounds->y)
        {
            minBounds->y = vertices.positions[i].y;
        }

        if (vertices.positions[i].z < minBounds->z)
        {
            minBounds->z = vertices.positions[i].z;
        }

        if (vertices.positions[i].x > maxBounds->x)
        {
            maxBounds->x = vertices.positions[i].x;
        }

        if (vertices.positions[i].y > maxBounds->y)
        {
            maxBounds->y = vertices.positions[i].y;
        }

        if (vertices.positions[i].z > maxBounds->z)
        {
            maxBounds->z = vertices.positions[i].z;
        }
    }

    // Now parse out the indices, as we can properly use the uvVertexRemapping list.
    for (const PosUvPair& pair : rawIndices)
    {
        unsigned int vertexIndex = GetActualVertexIndex(pair.positionId, pair.uvId, vertices);
        vertices.indices.push_back(vertexIndex);
    }

    return true;
}

unsigned int ModelManager::LoadModel(const char* rootFilename)
{
    std::stringstream combinationStream;
    combinationStream << rootFilename << ".png";
    std::string pngString = combinationStream.str();

    combinationStream.str("");
    combinationStream << rootFilename << ".obj";
    std::string objString = combinationStream.str();

    TextureModel textureModel;
    textureModel.textureId = imageManager->AddImage(pngString.c_str());
    if (textureModel.textureId == 0)
    {
        Logger::Log("Error loading the texture image!");
        Logger::LogError(pngString.c_str());
        return 0;
    }

    if (!LoadModel(objString.c_str(), textureModel.vertices, &textureModel.minBounds, &textureModel.maxBounds))
    {
        Logger::Log("Error loading the OBJ model!");
        Logger::LogError(objString.c_str());
        return 0;
    }

    models[nextModelId] = textureModel;
    ++nextModelId;
    return nextModelId - 1;
}

const TextureModel& ModelManager::GetModel(unsigned int id)
{
    return models[id];
}

unsigned int ModelManager::GetCurrentModelCount() const
{
    return nextModelId;
}

void ModelManager::RenderModel(vec::mat4& projectionMatrix, unsigned int id, vec::mat4& mvMatrix, bool selected)
{
    glUseProgram(modelRenderProgram);

    GLuint unit = 0;
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, models[id].textureId);
    glUniform1i(textureLocation, unit);

    glUniformMatrix4fv(projLocation, 1, GL_FALSE, projectionMatrix);
    glUniformMatrix4fv(mvLocation, 1, GL_FALSE, mvMatrix);
    glUniform1f(selectionFactorLocation, selected ? 0.40f : 0.0f);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, models[id].vertices.indices.size(), GL_UNSIGNED_INT, (const void*)(models[id].indexOffset * sizeof(GL_UNSIGNED_INT)));
}

// Initializes the OpenGL resources
bool ModelManager::InitializeOpenGlResources(ShaderManager& shaderManager)
{
    if (!shaderManager.CreateShaderProgram("modelRender", &modelRenderProgram))
    {
        Logger::Log("Error creating the model shader!");
        return false;
    }

    textureLocation = glGetUniformLocation(modelRenderProgram, "modelTexture");
    mvLocation = glGetUniformLocation(modelRenderProgram, "mvMatrix");
    projLocation = glGetUniformLocation(modelRenderProgram, "projMatrix");
    selectionFactorLocation = glGetUniformLocation(modelRenderProgram, "selectionFactor");

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &positionBuffer);
    glGenBuffers(1, &uvBuffer);
    glGenBuffers(1, &indexBuffer);

    return true;
}

// Sends in the model data to OpenGL.
void ModelManager::ResetOpenGlModelData()
{
    glBindVertexArray(vao);

    universalVertices temporaryCopyVertices;

    unsigned int indexPositionReferralOffset = 0;
    unsigned int indexOffset = 0;
    for (unsigned int i = 0; i < models.size(); i++)
    {
        models[i].indexOffset = indexOffset;
        temporaryCopyVertices.positions.insert(temporaryCopyVertices.positions.end(), models[i].vertices.positions.begin(), models[i].vertices.positions.end());
        temporaryCopyVertices.uvs.insert(temporaryCopyVertices.uvs.end(), models[i].vertices.uvs.begin(), models[i].vertices.uvs.end());

        for (unsigned int j = 0; j < models[i].vertices.indices.size(); j++)
        {
            temporaryCopyVertices.indices.push_back(models[i].vertices.indices[j] + indexPositionReferralOffset);
        }

        indexPositionReferralOffset += models[i].vertices.positions.size();
        indexOffset += models[i].vertices.indices.size();
    }

    temporaryCopyVertices.TransferPositionToOpenGl(positionBuffer);
    temporaryCopyVertices.TransferUvsToOpenGl(uvBuffer);
    temporaryCopyVertices.TransferIndicesToOpenGl(indexBuffer);
}

// Deletes all initialized OpenGL resources.
ModelManager::~ModelManager()
{
    glDeleteVertexArrays(1, &vao);

    glDeleteBuffers(1, &positionBuffer);
    glDeleteBuffers(1, &uvBuffer);
    glDeleteBuffers(1, &indexBuffer);
}
