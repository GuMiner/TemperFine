#pragma once
#include <vector>
#include "MapInfo.h"

// Manages reading maps.
class MapManager
{
    public:
        MapManager();
        bool ReadMap(const char* filename, MapInfo& outputMap);
    protected:
    private:
};
