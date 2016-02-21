#include "VoxelRoute.h"

// Given a voxel, finds all valid voxel neighbors for travel. Returns true if neighbors were found, false otherwise.
void VoxelRouteRules::FindVoxelNeighbors(const MapInfo& voxelMap, const vec::vec3i& voxelId, std::vector<vec::vec3i>& neighbors)
{
    // Valid neighbors for a voxel are to the immediate sides (cube, slant, correctly oriented) and to the immediate sides + 1 height (correctly-oriented slants)
    // Valid neighbors must also not be covered with anything other than air above the voxel.
    // Slants:
    // Orientation 0: Allows X- travel up, X+ travel down.
    // Orientation 1: Y- travel up, Y+ travel down.
    // 2: Reverse of 0
    // 3: Reverse of 1
    // 4-7: Acts as a flat.
    // Additionally, slants can travel to slants on the immediate sides (same inclination), but not flats (different inclination)

    vec::vec3i xMinus = vec::vec3i(voxelId.x - 1, voxelId.y, voxelId.z);
    vec::vec3i xPlus = vec::vec3i(voxelId.x + 1, voxelId.y, voxelId.z);
    vec::vec3i yMinus = vec::vec3i(voxelId.x, voxelId.y - 1, voxelId.z);
    vec::vec3i yPlus = vec::vec3i(voxelId.x, voxelId.y + 1, voxelId.z);

    vec::vec3i xMinusPlusZ = vec::vec3i(voxelId.x - 1, voxelId.y, voxelId.z + 1);
    vec::vec3i xPlusPlusZ = vec::vec3i(voxelId.x + 1, voxelId.y, voxelId.z + 1);
    vec::vec3i yMinusPlusZ = vec::vec3i(voxelId.x, voxelId.y - 1, voxelId.z + 1);
    vec::vec3i yPlusPlusZ = vec::vec3i(voxelId.x, voxelId.y + 1, voxelId.z + 1);

    int type = voxelMap.GetType(voxelId);
    bool cubeType = (type == MapInfo::VoxelTypes::CUBE) || (type == MapInfo::VoxelTypes::SLANT && voxelMap.GetOrientation(voxelId) > 3);
    if (cubeType)
    {
        // Check voxels on the same level as the cube.
        AddNeighborIfValidFromFlatVoxel(voxelMap, xMinus, 2, neighbors);
        AddNeighborIfValidFromFlatVoxel(voxelMap, xPlus, 0, neighbors);
        AddNeighborIfValidFromFlatVoxel(voxelMap, yMinus, 3, neighbors);
        AddNeighborIfValidFromFlatVoxel(voxelMap, yPlus, 1, neighbors);

        // Check voxels up one level, which can only be slants atm.
        AddNeighborIfValidSlant(voxelMap, xMinusPlusZ, 0, neighbors);
        AddNeighborIfValidSlant(voxelMap, xPlusPlusZ, 2, neighbors);
        AddNeighborIfValidSlant(voxelMap, yMinusPlusZ, 1, neighbors);
        AddNeighborIfValidSlant(voxelMap, yPlusPlusZ, 3, neighbors);
    }
    else if (type == MapInfo::VoxelTypes::SLANT)
    {
        // The slant isn't a cube-type slant, so check the side voxels based on the slant orientation.
        int slantOrientation = voxelMap.GetOrientation(voxelId);
        if (slantOrientation == 0 || slantOrientation == 2)
        {
            // X direction, same level checks
            AddNeighborIfValidSlant(voxelMap, yMinus, slantOrientation, neighbors);
            AddNeighborIfValidSlant(voxelMap, yPlus, slantOrientation, neighbors);
        }
        else
        {
            // Y direction, same level checks
            AddNeighborIfValidSlant(voxelMap, xMinus, slantOrientation, neighbors);
            AddNeighborIfValidSlant(voxelMap, xPlus, slantOrientation, neighbors);
        }

        // Check the voxel above and voxel below. Note that valid slants must be the same orientation for continual travel.
        vec::vec3i xMinusMinusZ = vec::vec3i(voxelId.x - 1, voxelId.y, voxelId.z - 1);
        vec::vec3i xPlusMinusZ = vec::vec3i(voxelId.x + 1, voxelId.y, voxelId.z - 1);
        vec::vec3i yMinusMinusZ = vec::vec3i(voxelId.x, voxelId.y - 1, voxelId.z - 1);
        vec::vec3i yPlusMinusZ = vec::vec3i(voxelId.x, voxelId.y + 1, voxelId.z - 1);

        switch (slantOrientation)
        {
        case 0:
            AddNeighborIfValidFromFlatVoxel(voxelMap, xMinus, slantOrientation, neighbors);
            AddNeighborIfValidFromFlatVoxel(voxelMap, xMinusMinusZ, slantOrientation, neighbors);
            break;
        case 1:
            AddNeighborIfValidFromFlatVoxel(voxelMap, yMinus, slantOrientation, neighbors);
            AddNeighborIfValidFromFlatVoxel(voxelMap, yMinusMinusZ, slantOrientation, neighbors);
            break;
        case 2:
            AddNeighborIfValidFromFlatVoxel(voxelMap, xPlus, slantOrientation, neighbors);
            AddNeighborIfValidFromFlatVoxel(voxelMap, xPlusMinusZ, slantOrientation, neighbors);
            break;
        case 3:
            AddNeighborIfValidFromFlatVoxel(voxelMap, yPlus, slantOrientation, neighbors);
            AddNeighborIfValidFromFlatVoxel(voxelMap, yPlusMinusZ, slantOrientation, neighbors);
            break;
        default:
            break;
        }
    }
}

// Returns true if a voxel is minimally accessible, which means it is within bounds and has air above it.
bool VoxelRouteRules::IsVoxelMinimallyAccessible(const MapInfo& voxelMap, const vec::vec3i& voxelId)
{
    if (voxelMap.InBounds(voxelId))
    {
        // Is there air above the voxel?
        vec::vec3i aboveVoxel = vec::vec3i(voxelId.x, voxelId.y, voxelId.z + 1);
        if (!voxelMap.InBounds(aboveVoxel) || voxelMap.GetType(aboveVoxel) == MapInfo::VoxelTypes::AIR)
        {
            return true;
        }
    }

    return false;
}

// Adds the neighbor if it is a valid slant. Intended for travel-up slants from flat areas.
void VoxelRouteRules::AddNeighborIfValidSlant(const MapInfo& voxelMap, const vec::vec3i& neighborVoxelId, int validSlantOrientation, std::vector<vec::vec3i>& neighbors)
{
    // Is the voxel valid to travel to?
    if (IsVoxelMinimallyAccessible(voxelMap, neighborVoxelId))
    {
        int neighborType = voxelMap.GetType(neighborVoxelId);
        if (neighborType == MapInfo::VoxelTypes::SLANT && voxelMap.GetOrientation(neighborVoxelId) == validSlantOrientation)
        {
            neighbors.push_back(neighborVoxelId);
        }
    }
}

// Determines if a given voxel is valid given a starting flat voxel. If so, adds it to the list of neighbors.
void VoxelRouteRules::AddNeighborIfValidFromFlatVoxel(const MapInfo& voxelMap, const vec::vec3i& neighborVoxelId, int validSlantOrientation, std::vector<vec::vec3i>& neighbors)
{
    // Is the voxel valid to travel to?
    bool isValidNeighbor = false;
    if (IsVoxelMinimallyAccessible(voxelMap, neighborVoxelId))
    {
        // Is the voxel oriented properly?
        int neighborType = voxelMap.GetType(neighborVoxelId);
        if (neighborType == MapInfo::VoxelTypes::CUBE)
        {
            isValidNeighbor = true;
        }
        else if (neighborType == MapInfo::VoxelTypes::SLANT)
        {
            if (voxelMap.GetOrientation(neighborVoxelId) == validSlantOrientation)
            {
                isValidNeighbor = true;
            }
        }
    }

    if (isValidNeighbor)
    {
        neighbors.push_back(neighborVoxelId);
    }
}
