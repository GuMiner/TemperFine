#include <cstring>
#include <sstream>
#include "Logger.h"
#include "TechConfig.h"
#include "StringUtils.h"

std::vector<Tech> TechConfig::Techs;

bool TechConfig::LoadConfigValues(std::vector<std::string>& configFileLines)
{
    // Move from the ConfigVersion line to actual data.
    ++lineCounter;

    int linesPerTech = 3;
    while (lineCounter + linesPerTech <= (int)configFileLines.size())
    {
        // Load in the technology
        Tech tech;
        tech.names.push_back(std::string(configFileLines[lineCounter]));

        std::stringstream loadingMsg;
        loadingMsg << "Loading technology \"" << tech.names[0] << "\".";
        Logger::Log(loadingMsg.str().c_str());

        // Load in all the minor armor details, after breaking apart the three detail lines.
        std::vector<std::string> resources;
        std::vector<std::string> dependencies;

        StringUtils::Split(configFileLines[lineCounter + 1], StringUtils::Space, true, resources);
        if (resources.size() != 3)
        {
            Logger::LogError("Expected 3 elements for a technology resource configuration line.");
            return false;
        }

        StringUtils::Split(configFileLines[lineCounter + 2], StringUtils::Space, true, dependencies);
        if (dependencies.size() < 1)
        {
            Logger::LogError("Expected at least one dependency for a technology resource configuration line.");
            return false;
        }

        std::stringstream imageName;
        imageName << "images/techs/" << resources[0] << ".png";
        tech.internalName = resources[0];
        if (!tech.techImage.loadFromFile(imageName.str()))
        {
            Logger::LogError("Error reading in the image for the technology.");
            return false;
        }

        if (!StringUtils::ParseIntFromString(resources[1], tech.researchTimeSeconds) ||
            !StringUtils::ParseFloatFromString(resources[2], tech.researchResources))
        {
            Logger::LogError("Error parsing the technology time and resource requirements.");
            return false;
        }

        // Given the dependencies, find the dependencies that were already loaded.
        std::vector<unsigned int> dependencyIds;
        for (unsigned int i = 0; i < dependencies.size(); i++)
        {
            bool foundDependency = false;
            for (unsigned int j = 0; j < Techs.size(); j++)
            {
                if (Techs[j].internalName.size() == dependencies[i].size() &&
                    strncmp(Techs[j].internalName.c_str(), dependencies[i].c_str(), dependencies[i].size()) == 0)
                {
                    foundDependency = true;
                    dependencyIds.push_back(j);
                    break;
                }
            }

            if (strncmp(dependencies[i].c_str(), "none", 4) == 0)
            {
                // Exit if there is no dependency for this technology.
                foundDependency = true;
            }

            if (!foundDependency)
            {
                Logger::Log("Missing dependency for tech!");
                Logger::Log(dependencies[i].c_str());
                return false;
            }
        }

        tech.dependencies.push_back(dependencyIds);

        // Save the loaded armor information.
        Techs.push_back(tech);
        lineCounter += linesPerTech;
    }

    // TODO find techs with the same image name and merge them,
    //  as they represent the same tech reachable via different routes.

    return true;
}

void TechConfig::WriteConfigValues()
{
	// Cannot write to the technology configuration file.
}

TechConfig::TechConfig(const char* configName)
	: ConfigManager(configName)
{
}
