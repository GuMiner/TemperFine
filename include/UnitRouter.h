#pragma once
#include "MapInfo.h"
#include "MapSections.h"

// Performs unit routing refinement and visualization.
class UnitRouter
{
    public:
        UnitRouter();

        // Refines a route among the voxels to minimize 'zig zags' and travel in a nice, constant path (or rotary path) to the final destination.
        void RefineRoute(MapInfo* mapInfo, const voxelSubsectionsMap& voxelSubsections, const vec::vec3i start, const vec::vec3i destination,
            const std::vector<vec::vec3i>& givenPath, std::vector<vec::vec3i>& refinedPath, std::vector<vec::vec3>& visualPath);

        // Determines the height of a given voxel at the provided position for a unit.
        static float GetHeightForVoxel(MapInfo* voxelMap, const vec::vec3i& voxelId, const vec::vec3& position);

    private:
        // Returns true if the average distance of the given points (excluding the start and end point) is > restingDistance * (1.0f + maxPercentage);
        bool IsStretchedPercentage(float restingDistanceAvg, const std::vector<vec::vec3>& currentPoints, float maxPercentage);

        // Performs a spring-mass 'string' refinement to make our routes look nice
        // Updates the string route and refined integer path based on our string route.
        void PerformStringRefinement(MapInfo* mapInfo, const voxelSubsectionsMap& voxelSubsections, std::vector<vec::vec3>& stringRoute, std::vector<vec::vec3i>& refinedPath);
};
