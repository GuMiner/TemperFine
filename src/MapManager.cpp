#include <map>
#include <string>
#include "ConversionUtils.h"
#include "Logger.h"
#include "StringUtils.h"
#include "MapManager.h"

MapManager::MapManager()
{
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

    // First line is the configuration version.
    // TODO TODO

    return true;
}
