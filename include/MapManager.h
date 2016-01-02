#pragma once
#include <vector>
#include "MapInfo.h"

// Manages reading maps.
class MapManager
{
    public:
        MapManager();

        // Reads in a map file, filling in the MapInfo (if true is returned)
        bool ReadMap(const char* filename, MapInfo& outputMap);
    protected:
    private:
        int currentLine;

        // Loads in a segment of map block data into the provided array.
        bool LoadMapBlockData(std::vector<std::string>& lines, unsigned char* dataStorage, unsigned int xSize, unsigned int ySize, unsigned int zSize);
};
