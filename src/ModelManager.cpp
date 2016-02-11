#include <cstring>
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

        vmath::vec3 vector;
        if (!StringUtils::ParseFloatFromString(line[1], vector[0]) ||
            !StringUtils::ParseFloatFromString(line[2], vector[1]) ||
            !StringUtils::ParseFloatFromString(line[3], vector[2]))
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

        vmath::vec2 vector;
        if (!StringUtils::ParseFloatFromString(line[1], vector[0]) ||
            !StringUtils::ParseFloatFromString(line[2], vector[1]))
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
        }
    }

    return true;
}

bool ModelManager::LoadModel(const char* objFilename, universalVertices& vertices, vmath::vec3* minBounds, vmath::vec3* maxBounds)
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
    (*minBounds)[0] = 1e20;
    (*minBounds)[1] = 1e20;
    (*minBounds)[2] = 1e20;
    (*maxBounds)[0] = -1e20;
    (*maxBounds)[1] = -1e20;
    (*maxBounds)[2] = -1e20;

    // There's guaranteed to be a UV for each point. Find it, and set it.
    for (unsigned int i = 0; i < vertices.positions.size(); i++)
    {
        bool foundUv = false;
        for (const PosUvPair& pair : rawIndices)
        {
            if (pair.positionId == i)
            {
                foundUv = true;
                vertices.uvs.push_back(rawUvs[pair.uvId]);

                // Initialize the remapping for this index.
                uvVertexRemapping[i] = std::vector<PosUvPair>();
                uvVertexRemapping[i].push_back(pair);
                break;
            }
        }

        if (!foundUv)
        {
            std::stringstream errorStream;
            errorStream << "Failed to load in the UV for point " << i << ".";
            Logger::Log(errorStream.str().c_str());
            return false;
        }

        // Perform min-max bounding check.
        if (vertices.positions[i][0] < (*minBounds)[0])
        {
            (*minBounds)[0] = vertices.positions[i][0];
        }

        if (vertices.positions[i][1] < (*minBounds)[1])
        {
            (*minBounds)[1] = vertices.positions[i][1];
        }

        if (vertices.positions[i][2] < (*minBounds)[2])
        {
            (*minBounds)[2] = vertices.positions[i][2];
        }

        if (vertices.positions[i][0] > (*maxBounds)[0])
        {
            (*maxBounds)[0] = vertices.positions[i][0];
        }

        if (vertices.positions[i][1] > (*maxBounds)[1])
        {
            (*maxBounds)[1] = vertices.positions[i][1];
        }

        if (vertices.positions[i][2] > (*maxBounds)[2])
        {
            (*maxBounds)[2] = vertices.positions[i][2];
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

void ModelManager::RenderModel(vmath::mat4& projectionMatrix, unsigned int id, vmath::mat4& mvMatrix)
{
    // TEST CODE TODO REMOVE
    glUseProgram(modelRenderProgram);

    GLuint unit = 0;
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, models[id].textureId);
    glUniform1i(textureLocation, unit);

    glUniformMatrix4fv(projLocation, 1, GL_FALSE, projectionMatrix);
    glUniformMatrix4fv(mvLocation, 1, GL_FALSE, mvMatrix);

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
