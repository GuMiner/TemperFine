#include <set>
#include <queue>
#include "Logger.h"
#include "MapSections.h"
#include "MathOps.h"

MapSections::MapSections()
{
    needsRecomputation = true;
}

// Recomputes the map sections to use for routing.
void MapSections::RecomputeMapSections(MapInfo* mapInfo)
{
    Logger::Log("Recomputing map sections...");
    int nextSubsectionId = 0;

    subsections.clear();
    for (unsigned int z = 0; z < mapInfo->zSize; z++)
    {
        for (unsigned int y = 0; y < mapInfo->ySize; y++)
        {
            for (unsigned int x = 0; x < mapInfo->xSize; x++)
            {
                if (mapInfo->blockType[mapInfo->GetIndex(x, y, z)] == MapInfo::VoxelTypes::AIR)
                {
                    // Air voxels cannot be traveled on top of.
                    continue;
                }
                else if (subsections.count(vmath::vec3i(x, y, z)) != 0)
                {
                    // Voxel already in a map somewhere, skip.
                    continue;
                }
                else
                {
                    // This is a new voxel that isn't in a route. First, verify it can be in a route
                    vmath::vec3i voxelId = vmath::vec3i(x, y, z);
                    if (VoxelRouteRules::IsVoxelMinimallyAccessible(mapInfo, voxelId))
                    {
                        std::set<vmath::vec3i, vmath::vec3iComparer> neighborsToSearch;
                        neighborsToSearch.insert(voxelId);

                        Logger::Log("Adding new voxel section!");
                        unsigned int voxelsAddedInSection = 0;

                        // Performs a BFS search of the space
                        while (neighborsToSearch.size() != 0)
                        {
                            vmath::vec3i currentVoxelId = *neighborsToSearch.begin();
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

    needsRecomputation = false;
}

// Computes a route between two points using the map sections. Returns true (and fills in the path) if a path was found, false otherwise.
bool MapSections::ComputeRoute(const vmath::vec3i start, const vmath::vec3i destination, std::vector<vmath::vec3i>& path)
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
    std::set<vmath::vec3i, vmath::vec3iComparer> foundVoxels;

    // Voxels on the edge of search radius, to use for future searches.
    std::queue<vmath::vec3i> edgeVoxels;

    // The current route, from start to end, for each voxel.
    std::map<vmath::vec3i, std::vector<vmath::vec3i>, vmath::vec3iComparer> voxelRoutes;
    voxelRoutes[start] = std::vector<vmath::vec3i>();
    voxelRoutes[start].push_back(start);

    edgeVoxels.push(start);
    foundVoxels.insert(start);

    while (edgeVoxels.size() != 0)
    {
        vmath::vec3i voxelId = edgeVoxels.front();
        edgeVoxels.pop();
        const VoxelRoute& voxelInfo = subsections[voxelId];

        // Add all the neighbors, if not already present, with added route, to the voxel route.
        for (unsigned int i = 0; i < voxelInfo.neighbors.size(); i++)
        {
            const vmath::vec3i& neighborVoxelId = voxelInfo.neighbors[i];
            if (foundVoxels.find(neighborVoxelId) == foundVoxels.end())
            {
                // New voxel. This neighbor is added to the active edge, found voxels, and has its route updated.
                foundVoxels.insert(neighborVoxelId);
                edgeVoxels.push(neighborVoxelId);

                std::vector<vmath::vec3i> voxelRoute = voxelRoutes[voxelId];
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

// True if the map needs to be recomputed, false otherwise.
bool MapSections::NeedsRecomputation() const
{
    return needsRecomputation;
}

bool MapSections::HitByRay(MapInfo* mapInfo, const vmath::vec3& rayStart, const vmath::vec3& rayVector, vmath::vec3i* voxelId)
{
    // First, figure out if the ray will hit the voxel area.
    vmath::vec3 minVoxelArea = vmath::vec3(0.0f, 0.0f, 0.0f);
    vmath::vec3 maxVoxelArea = vmath::vec3(MapInfo::SPACING * mapInfo->xSize, MapInfo::SPACING * mapInfo->ySize, MapInfo::SPACING * mapInfo->zSize);
    if (MathOps::WithinRange(rayStart, minVoxelArea, maxVoxelArea))
    {
        // Start position is within the voxel box, so it definitely intersects it.
    }

    return false;
}
