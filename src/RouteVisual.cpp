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
int RouteVisual::CreateRouteVisual(const std::vector<vec::vec3>& route)
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
void RouteVisual::DeleteRouteVisual(int routeId)
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

void RouteVisual::SendRoutesToOpenGl()
{
    glBindVertexArray(vao);
    routeVerties.TransferPositionToOpenGl(positionBuffer);
}

RouteVisual::~RouteVisual()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &positionBuffer);
}
