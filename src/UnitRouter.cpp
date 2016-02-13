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
void UnitRouter::Render(vec::mat4& projectionMatrix, int routeId, bool selected)
{
    glUseProgram(routeVisualProgram);
    glBindVertexArray(vao);

    glUniformMatrix4fv(projMatrixLocation, 1, GL_FALSE, projectionMatrix);

    // TODO use selected to visualize routes that are selected.
    glDrawArrays(GL_LINE_STRIP, routeData[routeId].offset, routeData[routeId].count);
}

// Creates a visual for the specified route.
int UnitRouter::CreateRouteVisual(const std::vector<vec::vec3>& route)
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

void UnitRouter::RefineRoute(const voxelSubsectionsMap& voxelSubsections, const vec::vec3i start, const vec::vec3i destination,
    const std::vector<vec::vec3i>& givenPath, std::vector<vec::vec3i>& refinedPath, std::vector<vec::vec3>& visualPath)
{
    const vec::vec3 offsetSpacing = vec::vec3(MapInfo::SPACING / 2.0f, MapInfo::SPACING / 2.0f, MapInfo::SPACING * 1.10f);

    if (givenPath.size() == 0)
    {
        // No path, no data.
        return;
    }
    else if (givenPath.size() < 3)
    {
        // Simple path.
        refinedPath = givenPath;

        // Perform direct scaling from the refined path.
        for (unsigned int i = 0; i < refinedPath.size(); i++)
        {
            visualPath.push_back(vec::vec3(refinedPath[i].x * MapInfo::SPACING, refinedPath[i].y * MapInfo::SPACING, refinedPath[i].z * MapInfo::SPACING) + offsetSpacing);
        }
    }
    else
    {
        // Perform physics-based refinement.
        //   The route is currently a right-angled, not-very-direct route.
        //   To refine the route, we create a spring-mass system ('string'), with a spring from each route point (mass).
        //   We then pull the string tight until the average distance between each mass is > starting amount * some factor.

        // Divide each segment in quarters to improve the resolution of the above.
        std::vector<vec::vec3> subdividedPath;
        subdividedPath.reserve(givenPath.size() * 4);
        subdividedPath.push_back(vec::vec3(givenPath[0].x, givenPath[0].y, givenPath[0].z));
        for (unsigned int i = 1; i < givenPath.size(); i++)
        {
            const vec::vec3 priorPoint = vec::vec3(givenPath[i - 1].x, givenPath[i - 1].y, givenPath[i - 1].z);
            const vec::vec3 difference = vec::vec3(givenPath[i].x, givenPath[i].y, givenPath[i].z) - priorPoint;

            subdividedPath.push_back(0.25f * difference + priorPoint);
            subdividedPath.push_back(0.50f * difference + priorPoint);
            subdividedPath.push_back(0.75f * difference + priorPoint);
            subdividedPath.push_back(difference + priorPoint);
        }

        // Perform our algorithm
        PerformStringRefinement(voxelSubsections, subdividedPath, refinedPath);

        // Save the (updated) integer path (for viability calculations) and scale up the floating-point path (actual travel and visibility calculations)
        for (const vec::vec3& point : subdividedPath)
        {
            visualPath.push_back((point * MapInfo::SPACING) + offsetSpacing);
        }
    }
}

// Performs a spring-mass 'string' refinement to make our routes look nice
// Updates the string route and refined integer path based on our string route.
void UnitRouter::PerformStringRefinement(const voxelSubsectionsMap& voxelSubsections, std::vector<vec::vec3>& stringRoute, std::vector<vec::vec3i>& refinedPath)
{
    // TODO TODO TODO


    // Save out the refined path based on on the subsection route.
    vec::vec3i priorPoint = vec::vec3i((int)stringRoute[0].x, (int)stringRoute[0].y, (int)stringRoute[0].z);
    refinedPath.push_back(priorPoint);
    for (const vec::vec3& point : stringRoute)
    {
        // Only save the point if it is in a different voxel than the prior floating-point value.
        vec::vec3i pointLocation = vec::vec3i((int)point.x, (int)point.y, (int)point.z);;
        if (!(priorPoint.x == pointLocation.x && priorPoint.y == pointLocation.y && priorPoint.z == pointLocation.z))
        {
            refinedPath.push_back(pointLocation);
            priorPoint = pointLocation;
        }
    }
}

UnitRouter::~UnitRouter()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &positionBuffer);
}
