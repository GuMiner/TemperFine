#include <map>
#include <string>
#include <sstream>
#include "ConversionUtils.h"
#include "Logger.h"
#include "StringUtils.h"
#include "MapManager.h"

MapManager::MapManager()
{
}

bool MapManager::LoadMapBlockData(std::vector<std::string>& lines, unsigned short* dataStorage, unsigned int xSize, unsigned int ySize, unsigned int zSize)
{
    std::stringstream errorStream;
    for (unsigned int k = 0; k < zSize; k++)
    {
        for (unsigned int j = 0; j < ySize; j++)
        {
            // Break apart our data line.
            std::vector<std::string> stringParts;
            StringUtils::Split(lines[currentLine++], StringUtils::Space, true, stringParts);

            if (stringParts.size() != xSize)
            {
                errorStream << "Incorrect element count in layer " << k << ", line " << j << ". Expecting " << xSize << "elements, not " << stringParts.size() << ".";
                Logger::LogError(errorStream.str().c_str());
                return false;
            }

            for (unsigned int i = 0; i < xSize; i++)
            {
                int value;
                if (!StringUtils::ParseIntFromString(stringParts[i], value))
                {
                    errorStream << "Couldn't decode a value in layer " << k << ", line " << j << ", element " << i << ".";
                    Logger::LogError(errorStream.str().c_str());
                    return false;
                }

                dataStorage[MapInfo::GetIndex(i, j, k, xSize, ySize)] = (unsigned short)value;
            }
        }
    }

    return true;
}

bool MapManager::ReadMap(const char* filename, MapInfo& outputMap)
{
    std::vector<std::string> lines;
    std::map<int, std::string> commentLines;
    if (!StringUtils::LoadConfigurationFile(filename, lines, commentLines))
    {
        Logger::Log("Unable to properly-parse the config-style file!");
        return false;
    }

    // First line is the config, followed by the map name.
    currentLine = 0;
    int mapVersion;
    if (!ConversionUtils::LoadInt(lines[currentLine++], mapVersion))
    {
        Logger::Log("Unable to properly load the map config version!");
        return false;
    }

    outputMap.mapConfigVersion = (unsigned int)mapVersion;
    outputMap.name = lines[currentLine++];

    std::stringstream mapName;
    mapName << "Loaded map \"" << outputMap.name << "\", config version " << outputMap.mapConfigVersion;
    Logger::Log(mapName.str().c_str());

    // Load the map size.
    int xSize, ySize, zSize;
    if (!ConversionUtils::LoadInt(lines[currentLine++], xSize))
    {
        Logger::Log("Unable to load the map X size!");
        return false;
    }

    if (!ConversionUtils::LoadInt(lines[currentLine++], ySize))
    {
        Logger::Log("Unable to load the map Y size!");
        return false;
    }

    if (!ConversionUtils::LoadInt(lines[currentLine++], zSize))
    {
        Logger::Log("Unable to load the map Z size!");
        return false;
    }

    outputMap.xSize = (unsigned int)xSize;
    outputMap.ySize = (unsigned int)ySize;
    outputMap.zSize = (unsigned int)zSize;

    int mapDataSize = outputMap.xSize * outputMap.ySize * outputMap.zSize;
    outputMap.blockType = new unsigned short[mapDataSize];
    outputMap.blockOrientation = new unsigned short[mapDataSize];
    outputMap.blockProperty = new unsigned short[mapDataSize];

    if (!LoadMapBlockData(lines, outputMap.blockType, outputMap.xSize, outputMap.ySize, outputMap.zSize))
    {
        Logger::Log("Unable to load the list of block types!");
        return false;
    }

    if (!LoadMapBlockData(lines, outputMap.blockOrientation, outputMap.xSize, outputMap.ySize, outputMap.zSize))
    {
        Logger::Log("Unable to load the list of block orientations!");
        return false;
    }

    if (!LoadMapBlockData(lines, outputMap.blockProperty, outputMap.xSize, outputMap.ySize, outputMap.zSize))
    {
        Logger::Log("Unable to load the list of block properties!");
        return false;
    }

    return true;
}
