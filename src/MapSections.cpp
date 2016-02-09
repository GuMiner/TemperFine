#include "MapSections.h"

MapSections::MapSections()
{
    needsRecomputation = true;
}

// Recomputes the map sections to use for routing.
void MapSections::RecomputeMapSections(MapInfo* mapInfo)
{
    subsections.clear();
    for (unsigned int z = 0; z < mapInfo->zSize; z++)
    {
        for (unsigned int y = 0; y < mapInfo->ySize; y++)
        {
            for (unsigned int x = 0; x < mapInfo->xSize; x++)
            {
                if (mapInfo->blockType[mapInfo->GetIndex(x, y, z)] == MapInfo::BlockTypes::AIR)
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
                    if (z != mapInfo->zSize - 1 && mapInfo->blockType[mapInfo->GetIndex(x, y, z + 1)] != MapInfo::BlockTypes::AIR)
                    {
                        // The voxel doesn't have air above it, so you cannot travel on top of this voxel.
                        continue;
                    }

                    // TODO perform a BFS of the nearby voxels, using the block type and orientation to determine neighbors.
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

    const VoxelRoute& startVoxel = subsections[start];
    const VoxelRoute& endVoxel = subsections[destination];

    if (startVoxel.subsectionId != endVoxel.subsectionId)
    {
        // The subsections these voxels are in differ, so there is no route from the two voxels.
        return false;
    }

    // TODO, at this point a search from the start to the end is guaranteed to find a route.
    return true;
}

// True if the map needs to be recomputed, false otherwise.
bool MapSections::NeedsRecomputation() const
{
    return needsRecomputation;
}
