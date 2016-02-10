#pragma once
#include <map>
#include <vector>
#include "MapInfo.h"
#include "vmath.hpp"

// Represents the possible routes traversable from a given voxel.
struct VoxelRoute
{
    vmath::vec3i voxelId;

    // Voxels that can be traveled to from this voxel.
    std::vector<vmath::vec3i> neighbors;

    // Which subsection this voxel is within.
    int subsectionId;
};

// Simplifies usages of the subsection map.
typedef std::map<vmath::vec3i, VoxelRoute, vmath::vec3iComparer> voxelSubsectionsMap;

// Computes the rules for how voxels are connected so that routing can be performed.
class VoxelRouteRules
{
    // Determines the height of a given voxel at the provided position.
    static float GetHeightForVoxel(MapInfo* voxelMap, const vmath::vec3i& voxelId, const vmath::vec3& position);

    // Given a voxel, finds all valid voxel neighbors for travel.
    static void FindVoxelNeighbors(MapInfo* voxelMap, const vmath::vec3i& voxelId, std::vector<vmath::vec3i>& neighbors);
};
