#pragma once
#include "MapInfo.h"
#include "VoxelRoute.h"

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

        // Accessor for the subsections.
        const voxelSubsectionsMap& GetSubsections() const;

        // Returns true if a voxel has been hit by the ray (and fills in the voxelId), false otherwise.
        bool HitByRay(MapInfo* mapInfo, const vmath::vec3& rayStart, const vmath::vec3& rayVector, vmath::vec3i* voxelId);

    private:
        bool needsRecomputation;
        voxelSubsectionsMap subsections;
};
