#pragma once
#include <map>
#include <vector>
#include "MapInfo.h"
#include "Vec.h"

// Represents the possible routes traversable from a given voxel.
struct VoxelRoute
{
    // Voxels that can be traveled to from this voxel.
    std::vector<vec::vec3i> neighbors;

    // Which subsection this voxel is within.
    int subsectionId;
};

// Computes the rules for how voxels are connected so that routing can be performed.
class VoxelRouteRules
{
public:
    // Given a voxel, finds all valid voxel neighbors for travel.
    static void FindVoxelNeighbors(const MapInfo& voxelMap, const vec::vec3i& voxelId, std::vector<vec::vec3i>& neighbors);

    // Returns true if a voxel is minimally accessible, which means it is within bounds and has air above it.
    static bool IsVoxelMinimallyAccessible(const MapInfo& voxelMap, const vec::vec3i& voxelId);

private:

    // Adds the neighbor if it is a valid slant. Intended for travel-up slants from flat areas.
    static void AddNeighborIfValidSlant(const MapInfo& voxelMap, const vec::vec3i& neighborVoxelId, int validSlantOrientation, std::vector<vec::vec3i>& neighbors);

    // Determines if a given voxel is valid given a starting flat voxel. If so, adds it to the list of neighbors.
    static void AddNeighborIfValidFromFlatVoxel(const MapInfo& voxelMap, const vec::vec3i& neighborVoxelId, int validSlantOrientation, std::vector<vec::vec3i>& neighbors);

};
