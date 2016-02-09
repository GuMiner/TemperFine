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

// Represents the sections of the map that units can travel in.
class MapSections
{
    public:
        MapSections();

        // Recomputes the map sections to use for routing.
        void RecomputeMapSections(MapInfo* mapInfo);

        // Computes a route between two points using the map sections. Returns true (and fills in the path) if a path was found, false otherwise.
        bool ComputeRoute(const vmath::vec3i start, const vmath::vec3i destination, std::vector<vmath::vec3i>& path);

        // True if the map needs to be recomputed, false otherwise.
        bool NeedsRecomputation() const;
    private:
        bool needsRecomputation;
        std::map<vmath::vec3i, VoxelRoute, vmath::vec3iComparer> subsections;
};
