#pragma once
#include "VoxelRoute.h"

// Performs unit routing calculations, using the MapSections route data as a starting point.
class UnitRouter
{
    public:
        UnitRouter();

        // Refines a route among the voxels to minimize 'zig zags' and travel in a nice, constant path (or rotary path) to the final destination.
        void RefineRoute(voxelSubsectionsMap* voxelSubsections, const vmath::vec3i start, const vmath::vec3i destination,
            const std::vector<vmath::vec3i>& givenPath, std::vector<vmath::vec3i>& refinedPath, std::vector<vmath::vec3>& visualPath);

    private:
};
