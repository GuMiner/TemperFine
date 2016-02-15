#pragma once
#include <map>
#include <vector>
#include <GL\glew.h>
#include "ShaderManager.h"
#include "Vec.h"
#include "Vertex.h"

// Rather standard line route visual data.
struct RouteVisualData
{
    GLint offset;
    GLsizei count;
};

// Visualizes routes for units.
class RouteVisual
{
public:
    RouteVisual();
    
    // Initializes the route visualization shader.
    bool Initialize(ShaderManager& shaderManager);

    // Renders the specified route.
    void Render(vec::mat4& projectionMatrix, int routeId, bool selected);

    // Creates a visual for the specified route.
    int CreateRouteVisual(const std::vector<vec::vec3>& route);

    // Deletes the visual for the specified route.
    void DeleteRouteVisual(int routeId);

    ~RouteVisual();

private:
    // Sends route data to OpenGL.
    void SendRoutesToOpenGl();

    // OpenGL elements for route visualization.
    GLuint routeVisualProgram;
    GLuint projMatrixLocation;

    GLuint vao;
    GLuint positionBuffer;

    universalVertices routeVerties;

    // Maps a route ID to the offset for vertices in OpenGL for the route.
    int nextRouteId;
    std::map<int, RouteVisualData> routeData;
};

