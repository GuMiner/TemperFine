#include "RouteVisual.h"
#include "Logger.h"

RouteVisual::RouteVisual()
{
    nextRouteId = 0;
}

// Initializes the route visualization shader.
bool RouteVisual::Initialize(ShaderManager& shaderManager)
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
void RouteVisual::Render(vec::mat4& projectionMatrix, int routeId, bool selected)
{
    glUseProgram(routeVisualProgram);
    glBindVertexArray(vao);

    glUniformMatrix4fv(projMatrixLocation, 1, GL_FALSE, projectionMatrix);

    // TODO use selected to visualize routes that are selected.
    glDrawArrays(GL_LINE_STRIP, routeData[routeId].offset, routeData[routeId].count);
}

// Creates a visual for the specified route.
int RouteVisual::CreateRouteVisual(const std::vector<vec::vec3> route)
{
    RouteVisualData routeVisualData;
    routeVisualData.offset = routeVertices.positions.size();
    routeVisualData.count = route.size();

    routeVertices.positions.insert(routeVertices.positions.end(), route.begin(), route.end());
    SendRoutesToOpenGl();

    int routeId = nextRouteId++;

    routeData[routeId] = routeVisualData;
    Logger::Log("Adding route id ", routeId, " with ", routeVisualData.count, " elements and ", routeVisualData.offset, " offset.");
    return routeId;
}

// Deletes the visual for the specified route.
void RouteVisual::DeleteRouteVisual(int routeId)
{
    // Remove data from the map and universal vertices.
    const RouteVisualData routeVisualData = routeData[routeId];
    Logger::Log("Deleting route ", routeId, " with ", routeVisualData.count, " elements and ", routeVisualData.offset, " offset from array with ", routeVertices.positions.size(), " elements.");

    routeVertices.positions.erase(routeVertices.positions.begin() + routeVisualData.offset, routeVertices.positions.begin() + routeVisualData.offset + routeVisualData.count);
    routeData.erase(routeData.find(routeId));

    // Update all map entries to point to the correct map amount.
    for (std::map<int, RouteVisualData>::iterator iter = routeData.begin(); iter != routeData.end(); iter++)
    {
        if (iter->second.offset > routeVisualData.offset)
        {
            iter->second.offset -= routeVisualData.count;
        }
    }

    // Skip an OpenGL update if there are no routes left.
    if (routeVertices.positions.size() != 0)
    {
        SendRoutesToOpenGl();
    }
}

void RouteVisual::SendRoutesToOpenGl()
{
    glBindVertexArray(vao);
    routeVertices.TransferPositionToOpenGl(positionBuffer);
}

RouteVisual::~RouteVisual()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &positionBuffer);
}
