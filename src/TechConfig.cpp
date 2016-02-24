#include <cstring>
#include <sstream>
#include "Logger.h"
#include "TechConfig.h"
#include "StringUtils.h"

std::vector<Tech> TechConfig::Techs;
sf::Image TechConfig::NoCurrentTechImage;

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
        // Note that this assumes dependencies are in the proper order.
        std::vector<unsigned int> dependencyIds;
        for (unsigned int i = 0; i < dependencies.size(); i++)
        {
            bool foundDependency = false;
            for (unsigned int j = 0; j < Techs.size(); j++)
            {
                if (strcmp(Techs[j].internalName.c_str(), dependencies[i].c_str()) == 0)
                {
                    foundDependency = true;
                    dependencyIds.push_back(j);
                    break;
                }
            }

            if (strcmp(dependencies[i].c_str(), LEVEL_ZERO) == 0)
            {
                // Exit if there is no dependency for this technology.
                foundDependency = true;
            }

            if (!foundDependency)
            {
                Logger::Log("Missing dependency for tech! Ensure dependencies are properly ordered in the technology file.");
                Logger::Log(dependencies[i].c_str());
                return false;
            }
        }

        tech.dependencies.push_back(dependencyIds);

        // Save the loaded armor information.
        Techs.push_back(tech);
        lineCounter += linesPerTech;
    }

    CombineIdenticalTechs();
    AssignTechLevels();

    // Load the 'NoCurrentTech' image.
    if (!NoCurrentTechImage.loadFromFile("images/techs/NoCurrentTechImage.png"))
    {
        Logger::LogError("Error reading in the image for when a technology has not been selected!");
        return false;
    }

    return true;
}

void TechConfig::CombineIdenticalTechs()
{
    for (unsigned int i = 0; i < Techs.size() - 1; i++)
    {
        for (unsigned int j = i + 1; j < Techs.size(); j++)
        {
            if (strcmp(Techs[i].internalName.c_str(), Techs[j].internalName.c_str()) == 0)
            {
                // Techs are identical, combine them. Note that based on how we iterate through the arrays,
                //  each 'j' tech will only have one name/dependency pair.
                Techs[i].dependencies.push_back(Techs[j].dependencies[0]);
                Techs[i].names.push_back(Techs[j].names[0]);

                // Clear the duplicate tech and ensure we reparse the current element again.
                Techs.erase(Techs.begin() + j);
                --i;

                // Lower the dependency ID for all upstream consumer...
                for (unsigned int k = i + 1; k < Techs.size(); k++)
                {
                    // ... where their dependencies
                    for (unsigned int m = 0; m < Techs[k].dependencies[0].size(); m++)
                    {
                        // ... are more than or equal to the item we just removed.
                        if (Techs[k].dependencies[0][m] >= j)
                        {
                            Techs[k].dependencies[0][m]--;
                        }
                    }
                }

                break;
            }
        }
    }
}

void TechConfig::AssignTechLevels()
{
    // Note that we're heavily-simplifying the logic by assuming that techs are *always* in dependency-order.
    for (unsigned int i = 0; i < Techs.size(); i++)
    {
        // Mash together all the dependencies
        std::vector<unsigned int> dependencies;
        for (unsigned int j = 0; j < Techs[i].dependencies.size(); j++)
        {
            dependencies.insert(dependencies.end(), Techs[i].dependencies[0].begin(), Techs[i].dependencies[0].end());
        }

        int level = 0;
        for (unsigned int j = 0; j < dependencies.size(); j++)
        {
            if (Techs[dependencies[j]].techLevel >= level)
            {
                level = Techs[dependencies[j]].techLevel + 1;
            }
        }

        Techs[i].techLevel = level;
    }
}

void TechConfig::WriteConfigValues()
{
	// Cannot write to the technology configuration file.
}

TechConfig::TechConfig(const char* configName)
	: ConfigManager(configName)
{
}
