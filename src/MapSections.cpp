#include <set>
#include "Logger.h"
#include "MapSections.h"

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

                        Logger::Log("Added ", voxelsAddedInSection, "voxels to voxel subsection ", nextSubsectionId, ".");
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
    if (subsections.count(start) == 0 || subsections.count(destination) == 0)
    {
        // Voxels not found in the range of travellable voxels.
        // [Usually this occurs if a player clicks the *side* of the map or a structure.
        return false;
    }

    Logger::Log("Routing from (", start.x, ", ", start.y, ", ", start.x, ") to (", destination.x, ", ", destination.y, ", ", destination.z, ")...");
    const VoxelRoute& startVoxel = subsections[start];
    const VoxelRoute& endVoxel = subsections[destination];

    if (startVoxel.subsectionId != endVoxel.subsectionId)
    {
        // The subsections these voxels are in differ, so there is no route from the two voxels.
        return false;
    }

    // At this point, a search is guaranteed to find the ending voxel. So, perform the search.

    // Voxels already found in the search, not to be reconsidered.
    std::set<vmath::vec3i, vmath::vec3iComparer> foundVoxels;

    // Voxels on the edge of search radius, to use for future searches.
    std::set<vmath::vec3i, vmath::vec3iComparer> edgeVoxels;

    // The current route, from start to end, for each voxel.
    std::map<vmath::vec3i, std::vector<vmath::vec3i>, vmath::vec3iComparer> voxelRoutes;
    voxelRoutes[start] = std::vector<vmath::vec3i>();
    voxelRoutes[start].push_back(start);

    edgeVoxels.insert(start);
    foundVoxels.insert(start);

    while (edgeVoxels.find(destination) == edgeVoxels.end())
    {
        vmath::vec3i voxelId = *edgeVoxels.begin();
        const VoxelRoute& voxelInfo = subsections[voxelId];
        edgeVoxels.erase(edgeVoxels.begin());

        // Add all the neighbors, if not already present, with added route, to the voxel route.
        for (unsigned int i = 0; i < voxelInfo.neighbors.size(); i++)
        {
            const vmath::vec3i& neighborVoxelId = voxelInfo.neighbors[i];
            if (foundVoxels.find(neighborVoxelId) == foundVoxels.end())
            {
                // New voxel. This neighbor is added to the active edge, found voxels, and has its route updated.
                foundVoxels.insert(neighborVoxelId);
                edgeVoxels.insert(neighborVoxelId);

                std::vector<vmath::vec3i> voxelRoute = voxelRoutes[voxelId];
                voxelRoute.push_back(neighborVoxelId);
                voxelRoutes[neighborVoxelId] = voxelRoute;
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
