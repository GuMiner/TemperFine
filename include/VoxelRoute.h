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

// Simplifies usages of the subsection map.
typedef std::map<vec::vec3i, VoxelRoute, vec::vec3iComparer> voxelSubsectionsMap;

// Computes the rules for how voxels are connected so that routing can be performed.
class VoxelRouteRules
{
public:
    // Determines the height of a given voxel at the provided position.
    static float GetHeightForVoxel(MapInfo* voxelMap, const vec::vec3i& voxelId, const vec::vec3& position);

    // Given a voxel, finds all valid voxel neighbors for travel.
    static void FindVoxelNeighbors(MapInfo* voxelMap, const vec::vec3i& voxelId, std::vector<vec::vec3i>& neighbors);

    // Returns true if a voxel is minimally accessible, which means it is within bounds and has air above it.
    static bool IsVoxelMinimallyAccessible(MapInfo* voxelMap, const vec::vec3i& voxelId);

private:

    // Adds the neighbor if it is a valid slant. Intended for travel-up slants from flat areas.
    static void AddNeighborIfValidSlant(MapInfo* voxelMap, const vec::vec3i& neighborVoxelId, int validSlantOrientation, std::vector<vec::vec3i>& neighbors);

    // Determines if a given voxel is valid given a starting flat voxel. If so, adds it to the list of neighbors.
    static void AddNeighborIfValidFromFlatVoxel(MapInfo* voxelMap, const vec::vec3i& neighborVoxelId, int validSlantOrientation, std::vector<vec::vec3i>& neighbors);

};
