#pragma once
#include <GL/glew.h>
#include <map>
#include "ShaderManager.h"
#include "VoxelRoute.h"
#include "Vertex.h"

// Rather standard line route visual data.
struct RouteVisualData
{
    GLint offset;
    GLsizei count;
};

// Performs unit routing refinement and visualization.
class UnitRouter
{
    public:
        UnitRouter();

        // Initializes the route visualization shader.
        bool Initialize(ShaderManager& shaderManager);

        // Renders the specified route.
        void Render(vec::mat4& projectionMatrix, int routeId, bool selected);

        // Creates a visual for the specified route.
        int CreateRouteVisual(const std::vector<vec::vec3>& route);

        // Deletes the visual for the specified route.
        void DeleteRouteVisual(int routeId);

        ~UnitRouter();

        // Refines a route among the voxels to minimize 'zig zags' and travel in a nice, constant path (or rotary path) to the final destination.
        void RefineRoute(const voxelSubsectionsMap& voxelSubsections, const vec::vec3i start, const vec::vec3i destination,
            const std::vector<vec::vec3i>& givenPath, std::vector<vec::vec3i>& refinedPath, std::vector<vec::vec3>& visualPath);

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
