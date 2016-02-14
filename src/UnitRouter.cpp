#include "Logger.h"
#include "VecOps.h"
#include "VoxelRoute.h"
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

void UnitRouter::RefineRoute(MapInfo* mapInfo, const voxelSubsectionsMap& voxelSubsections, const vec::vec3i start, const vec::vec3i destination,
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
        //   The route is currently a right-angled, not-very-direct route.
        //   To refine the route, we create a spring-mass system ('string'), with a spring from each route point (mass).
        //   We then pull the string tight until the average distance between each mass is > starting amount * some factor.
        Logger::Log("Performing physics-based route refinement.");

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
        PerformStringRefinement(mapInfo, voxelSubsections, subdividedPath, refinedPath);

        // Save the (updated) integer path (for viability calculations) and scale up the floating-point path (actual travel and visibility calculations)
        for (const vec::vec3& point : subdividedPath)
        {
            visualPath.push_back((point * MapInfo::SPACING) + offsetSpacing);
        }
    }

    Logger::Log("Route refinement complete.");
}

// Performs a spring-mass 'string' refinement to make our routes look nice
// Updates the string route and refined integer path based on our string route.
void UnitRouter::PerformStringRefinement(MapInfo* mapInfo, const voxelSubsectionsMap& voxelSubsections, std::vector<vec::vec3>& stringRoute, std::vector<vec::vec3i>& refinedPath)
{
    float stretchinessDesired = 0.10f; // 10%

    // Lengths for which *after* this amount is pulled, a reverse-stretchiness force is applied. Think a rubber band, but unstretched
    std::vector<float> restingLengths;
    float avgRestingLengthDistance = 0.0f;
    for (unsigned int i = 1; i < stringRoute.size(); i++)
    {
        float distance = VecOps::Distance(stringRoute[i], stringRoute[i - 1]);

        // Note that we offset the resting distance to naturally gravitate towards our desired distance.
        restingLengths.push_back(distance / (1.0f + stretchinessDesired));
        avgRestingLengthDistance += distance;
    }

    avgRestingLengthDistance /= (stringRoute.size() - 2);

    const vec::vec3 start = stringRoute[0];
    const vec::vec3 end = stringRoute[stringRoute.size() - 1];

    // Note that we only allow relative motion on the
    std::vector<vec::vec3> velocities;
    for (unsigned int i = 1; i < stringRoute.size() - 1; i++)
    {
        velocities.push_back(vec::vec3(0.0f));
    }

    const float springConstant = 6.0f;
    const float simulationTimestep = 0.04f;
    const float dampeningConstant = 0.98f;
    const float mass = 1.0f;

    // 10% stretchiness.
    float currentDistance = avgRestingLengthDistance;
    unsigned int iterations = 0;
    Logger::Log("Starting physics-based route refinement with ", stringRoute.size(), " total nodes.");
    const unsigned int maxIterations = 100;
    while (!IsStretchedPercentage(avgRestingLengthDistance, stringRoute, stretchinessDesired) && iterations < maxIterations)
    {
        for (unsigned int i = 0; i < velocities.size(); i++)
        {
            vec::vec3 force = vec::vec3(0.0f);
            vec::vec3 negativeVector = stringRoute[i + 1] - stringRoute[i];
            vec::vec3 positiveVector = stringRoute[i + 2] - stringRoute[i + 1];
            force += -(springConstant) * (
                ((restingLengths[i] - vec::length(negativeVector)) * vec::normalize(negativeVector)) +
                ((restingLengths[i + 1] - vec::length(positiveVector)) * vec::normalize(positiveVector)));

            vec::vec3 accel = force / mass;

            // This is really stupid integration, but it gets the job done.
            velocities[i] += accel * simulationTimestep * dampeningConstant;
            stringRoute[i + 1] += velocities[i] * simulationTimestep;
        }

        ++iterations;
    }

    Logger::Log("Physics-based route refinement complete after ", iterations, " iterations, with ", stringRoute.size(), " nodes remaining.");
    if (iterations == maxIterations)
    {
        Logger::LogWarn("Hit the maximum number of iterations for route calculation! Route may appear odd.");
    }

    // TODO need to perform z-height updates.
    // VoxelRouteRules::GetHeightForVoxel()

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

// Returns true if the average distance of the given points (excluding the start and end point) is > restingDistance * (1.0f + maxPercentage);
bool UnitRouter::IsStretchedPercentage(float restingDistanceAvg, const std::vector<vec::vec3>& currentPoints, float maxPercentage)
{
    float currentDistance = 0.0f;
    for (unsigned int i = 1; i < currentPoints.size(); i++)
    {
        currentDistance += VecOps::Distance(currentPoints[i], currentPoints[i - 1]);
    }

    currentDistance /= (currentPoints.size() - 1);
    return currentDistance > restingDistanceAvg * (1.0f + maxPercentage);
}

UnitRouter::~UnitRouter()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &positionBuffer);
}
