#include <cmath>
#include <set>
#include <queue>
#include "Logger.h"
#include "MapSections.h"
#include "MathOps.h"
#include "PhysicsOps.h"

MapSections::MapSections()
{
}

// Recomputes the map sections to use for routing.
void MapSections::RecomputeMapSections(const MapInfo& mapInfo)
{
    Logger::Log("Recomputing map sections...");
    int nextSubsectionId = 0;

    subsections.clear();
    for (unsigned int z = 0; z < mapInfo.zSize; z++)
    {
        for (unsigned int y = 0; y < mapInfo.ySize; y++)
        {
            for (unsigned int x = 0; x < mapInfo.xSize; x++)
            {
                if (mapInfo.blockType[mapInfo.GetIndex(x, y, z)] == MapInfo::VoxelTypes::AIR)
                {
                    // Air voxels cannot be traveled on top of.
                    continue;
                }
                else if (subsections.count(vec::vec3i(x, y, z)) != 0)
                {
                    // Voxel already in a map somewhere, skip.
                    continue;
                }
                else
                {
                    // This is a new voxel that isn't in a route. First, verify it can be in a route
                    vec::vec3i voxelId = vec::vec3i(x, y, z);
                    if (VoxelRouteRules::IsVoxelMinimallyAccessible(mapInfo, voxelId))
                    {
                        std::set<vec::vec3i, vec::vec3iComparer> neighborsToSearch;
                        neighborsToSearch.insert(voxelId);

                        Logger::Log("Adding new voxel section!");
                        unsigned int voxelsAddedInSection = 0;

                        // Performs a BFS search of the space
                        while (neighborsToSearch.size() != 0)
                        {
                            vec::vec3i currentVoxelId = *neighborsToSearch.begin();
                            neighborsToSearch.erase(neighborsToSearch.begin());

                            if (subsections.count(currentVoxelId) != 0)
                            {
                                // Skip voxels that have already been processed.
                                continue;
                            }

                            // Find all neighbors for a voxel and store it.
                            VoxelRoute route;
                            route.subsectionId = nextSubsectionId;
                            VoxelRouteRules::FindVoxelNeighbors(mapInfo, currentVoxelId, route.neighbors);

                            subsections[currentVoxelId] = route;
                            ++voxelsAddedInSection;

                            // Add all found neighbors, if not already processed, into the neighbors list
                            for (unsigned int i = 0; i < route.neighbors.size(); i++)
                            {
                                if (subsections.count(route.neighbors[i]) == 0)
                                {
                                    neighborsToSearch.insert(route.neighbors[i]);
                                }
                            }
                        }

                        Logger::Log("Added ", voxelsAddedInSection, " voxels to voxel subsection ", nextSubsectionId, ".");
                        ++nextSubsectionId;
                    }
                }
            }
        }
    }
}

// Computes a route between two points using the map sections. Returns true (and fills in the path) if a path was found, false otherwise.
bool MapSections::ComputeRoute(const vec::vec3i start, const vec::vec3i destination, std::vector<vec::vec3i>& path)
{
    Logger::Log("Routing from (", start.x, ", ", start.y, ", ", start.x, ") to (", destination.x, ", ", destination.y, ", ", destination.z, ")...");
    if (subsections.count(start) == 0 || subsections.count(destination) == 0)
    {
        // Voxels not found in the range of travellable voxels.
        // [Usually this occurs if a player clicks the *side* of the map or a structure.
        Logger::Log("Routing failed, non-navigatable!");
        return false;
    }

    const VoxelRoute& startVoxel = subsections[start];
    const VoxelRoute& endVoxel = subsections[destination];

    if (startVoxel.subsectionId != endVoxel.subsectionId)
    {
        // The subsections these voxels are in differ, so there is no route from the two voxels.
        Logger::Log("Routing failed, subsection difference!");
        return false;
    }

    // At this point, a search is guaranteed to find the ending voxel. So, perform the search.

    // Voxels already found in the search, not to be reconsidered.
    std::set<vec::vec3i, vec::vec3iComparer> foundVoxels;

    // Voxels on the edge of search radius, to use for future searches.
    std::queue<vec::vec3i> edgeVoxels;

    // The current route, from start to end, for each voxel.
    std::map<vec::vec3i, std::vector<vec::vec3i>, vec::vec3iComparer> voxelRoutes;
    voxelRoutes[start] = std::vector<vec::vec3i>();
    voxelRoutes[start].push_back(start);

    edgeVoxels.push(start);
    foundVoxels.insert(start);

    while (edgeVoxels.size() != 0)
    {
        vec::vec3i voxelId = edgeVoxels.front();
        edgeVoxels.pop();
        const VoxelRoute& voxelInfo = subsections[voxelId];

        // Add all the neighbors, if not already present, with added route, to the voxel route.
        for (unsigned int i = 0; i < voxelInfo.neighbors.size(); i++)
        {
            const vec::vec3i& neighborVoxelId = voxelInfo.neighbors[i];
            if (foundVoxels.find(neighborVoxelId) == foundVoxels.end())
            {
                // New voxel. This neighbor is added to the active edge, found voxels, and has its route updated.
                foundVoxels.insert(neighborVoxelId);
                edgeVoxels.push(neighborVoxelId);

                std::vector<vec::vec3i> voxelRoute = voxelRoutes[voxelId];
                voxelRoute.push_back(neighborVoxelId);
                voxelRoutes[neighborVoxelId] = voxelRoute;

                if (neighborVoxelId.x == destination.x && neighborVoxelId.y == destination.y && neighborVoxelId.z == destination.z)
                {
                    // We found the destination!
                    break;
                }
            }
        }
    }

    // Search complete. Directly output the search data.
    path = voxelRoutes[destination];
    Logger::Log("Routing completed, with ", path.size(), " total steps from start to destination.");
    return true;
}

const voxelSubsectionsMap& MapSections::GetSubsections() const
{
    return subsections;
}

bool MapSections::HitByRay(MapInfo* mapInfo, const vec::vec3& rayStart, const vec::vec3& rayVector, vec::vec3i* voxelId)
{
    // First, figure out if the ray will hit the voxel area.
    vec::vec3 minVoxelArea = vec::vec3(0.0f, 0.0f, 0.0f);
    vec::vec3 maxVoxelArea = vec::vec3(mapInfo->GetXSize(), mapInfo->GetYSize(), mapInfo->GetZSize());
    if (MathOps::WithinRange(rayStart, minVoxelArea, maxVoxelArea))
    {
        // Start position is within the voxel box, so it definitely intersects it.
        // TODO iterate through the voxels.
        Logger::Log("Within range");
        return true;
    }
    else
    {
        // Check the five possible planes (4 sides + top) that the user could have hit.
        float intersectionFactor;
        if (PhysicsOps::HitsPlane(rayStart, rayVector, PhysicsOps::XY, vec::vec3(0.0f, 0.0f, mapInfo->GetZSize()), &intersectionFactor)
            && intersectionFactor > 0)
        {
            // Top
            vec::vec3 intersectionPoint = rayStart + rayVector * intersectionFactor;
            if (PhysicsOps::WithinSquare(intersectionPoint, PhysicsOps::Plane::XY, minVoxelArea, maxVoxelArea))
            {
                // Perform a voxel trace.
                return PerformVoxelTrace(mapInfo, rayStart, rayVector, PhysicsOps::XY, intersectionFactor, voxelId);
            }
        }

        float firstFactor, secondFactor;
        if (PhysicsOps::HitsPlane(rayStart, rayVector, PhysicsOps::YZ, vec::vec3(0.0f, 0.0f, 0.0f), &firstFactor) &&
            PhysicsOps::HitsPlane(rayStart, rayVector, PhysicsOps::YZ, vec::vec3(mapInfo->GetXSize(), 0.0f, 0.0f), &secondFactor) &&
            (firstFactor > 0 || secondFactor > 0))
        {
            float actualFactor = MathOps::SmallestPositive(firstFactor, secondFactor);

            // YZ min
            vec::vec3 intersectionPoint = rayStart + rayVector * actualFactor;
            if (PhysicsOps::WithinSquare(intersectionPoint, PhysicsOps::Plane::YZ, minVoxelArea, maxVoxelArea))
            {
                // Perform a voxel trace.
                return PerformVoxelTrace(mapInfo, rayStart, rayVector, PhysicsOps::YZ, actualFactor, voxelId);
            }
        }

        if (PhysicsOps::HitsPlane(rayStart, rayVector, PhysicsOps::XZ, vec::vec3(0.0f, 0.0f, 0.0f), &firstFactor) &&
            PhysicsOps::HitsPlane(rayStart, rayVector, PhysicsOps::XZ, vec::vec3(0.0f, mapInfo->GetYSize(), 0.0f), &secondFactor) &&
            (firstFactor > 0 || secondFactor > 0))
        {
            float actualFactor = MathOps::SmallestPositive(firstFactor, secondFactor);

            // XZ min
            vec::vec3 intersectionPoint = rayStart + rayVector * actualFactor;
            if (PhysicsOps::WithinSquare(intersectionPoint, PhysicsOps::Plane::XZ, minVoxelArea, maxVoxelArea))
            {
                // Perform a voxel trace.
                return PerformVoxelTrace(mapInfo, rayStart, rayVector, PhysicsOps::XZ, actualFactor, voxelId);
            }
        }
    }

    return false;
}

// Performs a trace through the known voxels, given that we know which plane it hit.
        // Returns true if the trace hits a non-air voxel (and fills in the voxel ID), false otherwise.
bool MapSections::PerformVoxelTrace(MapInfo* mapInfo, const vec::vec3& rayStart, const vec::vec3& rayVector,
    PhysicsOps::Plane plane, float intersectionPoint, vec::vec3i* foundVoxelId)
{
    // First, move in 1/20th of a unit so we're inside the voxel. If we're not, we're at the very very corner of a voxel.
    float wiggleAmount = MapInfo::SPACING / 20.0f;
    vec::vec3 currentPoint = rayStart + rayVector * (intersectionPoint + wiggleAmount);

    // Second, find the current voxel we're in.
    vec::vec3 voxelIdFloating = currentPoint / MapInfo::SPACING;
    vec::vec3i voxelId = vec::vec3i((int)voxelIdFloating.x, (int)voxelIdFloating.y, (int)voxelIdFloating.z);

    while (mapInfo->InBounds(voxelId))
    {
        if (mapInfo->GetType(voxelId) != MapInfo::VoxelTypes::AIR)
        {
            // We hit a non-air voxel
            *foundVoxelId = voxelId;
            Logger::Log("Selected voxel (", voxelId.x, ", ", voxelId.y, ", ", voxelId.z, ").");
            return true;
        }


        // We hit an air voxel. Iterate onwards.
        // Each iteration, we're going to hit one of four planes. Figure out which one, using the current point as the ray start.
        // The next one we hit is the lowest positive for range.
        float intersectionDistances[6];

        vec::vec3 voxelMin = MapInfo::SPACING * vec::vec3(voxelId.x, voxelId.y, voxelId.z);
        vec::vec3 voxelMax = MapInfo::SPACING * vec::vec3(voxelId.x + 1, voxelId.y + 1, voxelId.z + 1);

        // We can assume these all hit, it's rather inevitable, planes are pervasive.
        PhysicsOps::HitsPlane(currentPoint, rayVector, PhysicsOps::YZ, voxelMin, &intersectionDistances[0]);
        PhysicsOps::HitsPlane(currentPoint, rayVector, PhysicsOps::YZ, voxelMax, &intersectionDistances[1]);
        PhysicsOps::HitsPlane(currentPoint, rayVector, PhysicsOps::XZ, voxelMin, &intersectionDistances[2]);
        PhysicsOps::HitsPlane(currentPoint, rayVector, PhysicsOps::XZ, voxelMax, &intersectionDistances[3]);
        PhysicsOps::HitsPlane(currentPoint, rayVector, PhysicsOps::XY, voxelMin, &intersectionDistances[4]);
        PhysicsOps::HitsPlane(currentPoint, rayVector, PhysicsOps::XY, voxelMax, &intersectionDistances[5]);

        // Order (lower x, upper x, lower y, upper y, lower z, upper z)
        unsigned int minIndex = 0;
        float minDistance = MapInfo::SPACING * 10.0f; // Way outside of a voxel.
        for (unsigned int i = 0; i < 6; i++)
        {
            if (intersectionDistances[i] > 0 && intersectionDistances[i] < minDistance)
            {
                minIndex = i;
                minDistance = intersectionDistances[i];
            }
        }

        switch (minIndex)
        {
        case 0:
            // Iterate in the X- direction.
            voxelId.x--;
            break;
        case 1:
            // X+
            voxelId.x++;
            break;
        case 2:
            // Y-
            voxelId.y--;
            break;
        case 3:
            // Y+
            voxelId.y++;
            break;
        case 4:
            // Z-
            voxelId.z--;
            break;
        case 5:
            // Z+
            voxelId.z++;
            break;
        }

        currentPoint = currentPoint + rayVector * (minDistance + wiggleAmount);
    }

    return false;
}
