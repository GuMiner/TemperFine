#include "Logger.h"
#include "UnitRouter.h"

UnitRouter::UnitRouter()
{
    nextRouteId = 0;
}

// Initializes the route visualization shader.
bool UnitRouter::Initialize(ShaderManager& shaderManager)
{
    // Route program.
    if (!shaderManager.CreateShaderProgram("routeRender", &routeVisualProgram))
    {
        Logger::Log("Failure creating the route shader program!");
        return false;
    }

    projMatrixLocation = glGetUniformLocation(routeVisualProgram, "projMatrix");

    // General OpenGL resources.
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &positionBuffer);
    return true;
}

// Renders the specified route.
void UnitRouter::Render(vmath::mat4& projectionMatrix, int routeId, bool selected)
{
    glUseProgram(routeVisualProgram);
    glBindVertexArray(vao);

    glUniformMatrix4fv(projMatrixLocation, 1, GL_FALSE, projectionMatrix);

    // TODO use selected to visualize routes that are selected.
    glDrawArrays(GL_LINE_STRIP, routeData[routeId].offset, routeData[routeId].count);
}

// Creates a visual for the specified route.
int UnitRouter::CreateRouteVisual(const std::vector<vmath::vec3>& route)
{
    RouteVisualData routeVisualData;
    routeVisualData.offset = routeVerties.positions.size();
    routeVisualData.count = route.size();

    routeVerties.positions.insert(routeVerties.positions.end(), route.begin(), route.end());
    SendRoutesToOpenGl();

    int routeId = nextRouteId++;

    routeData[routeId] = routeVisualData;
    return routeId;
}

// Deletes the visual for the specified route.
void UnitRouter::DeleteRouteVisual(int routeId)
{
    // const RouteVisualData& routeVisualData = routeData[routeId];

    // TODO data needs to be remove from universalVertices.
    routeData.erase(routeId);

    // Skip an OpenGL update if there are no routes left.
    if (routeVerties.positions.size() != 0)
    {
        SendRoutesToOpenGl();
    }
}

void UnitRouter::SendRoutesToOpenGl()
{
    glBindVertexArray(vao);
    routeVerties.TransferPositionToOpenGl(positionBuffer);
}

void UnitRouter::RefineRoute(const voxelSubsectionsMap& voxelSubsections, const vmath::vec3i start, const vmath::vec3i destination,
    const std::vector<vmath::vec3i>& givenPath, std::vector<vmath::vec3i>& refinedPath, std::vector<vmath::vec3>& visualPath)
{
    // TODO do some real refinement.
    refinedPath = givenPath;

    // Scales and moves a refined path so it's visible.
    for (unsigned int i = 0; i < refinedPath.size(); i++)
    {
        const vmath::vec3 offsetSpacing = vmath::vec3(MapInfo::SPACING / 2.0f, MapInfo::SPACING / 2.0f, MapInfo::SPACING * 1.10f);
        visualPath.push_back(vmath::vec3(refinedPath[i].x * MapInfo::SPACING, refinedPath[i].y * MapInfo::SPACING, refinedPath[i].z * MapInfo::SPACING) + offsetSpacing);
    }
}

UnitRouter::~UnitRouter()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &positionBuffer);
}
