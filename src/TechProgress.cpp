#include <limits>
#include "Logger.h"
#include "TechConfig.h"
#include "TechProgress.h"

// Sets an empty tech progress with a user who just has the basic tank.
TechProgress::TechProgress()
{
    minTechLevel = 0;
    maxTechLevel = 0;

    completedTechs.insert(0); // First tech == basic tank.
    UpdateTechs();
}

// Completes a tech.
void TechProgress::CompleteTech(unsigned int tech)
{
    // Tech is only completed if it can be researched.
    if (techsAvailableForResearch.find(tech) != techsAvailableForResearch.end())
    {
        completedTechs.insert(tech);
        techsAvailableForResearch.erase(tech);
        UpdateTechs();
        Logger::Log("Player completed tech ", tech, ", ", TechConfig::Techs[tech].internalName, ".");
    }
}

// True if the tech exists, false otherwise.
bool TechProgress::HasTech(const char* techInternalName)
{
    for (std::set<unsigned int>::iterator iter = completedTechs.begin(); iter != completedTechs.end(); iter++)
    {
        if (strcmp(TechConfig::Techs[*iter].internalName.c_str(), techInternalName) == 0)
        {
            return true;
        }
    }

    return false;
}

bool TechProgress::HasTech(unsigned int techId)
{
    return completedTechs.find(techId) != completedTechs.end();
}

// Updates the min/max current tech range and techs available for research.
void TechProgress::UpdateTechs()
{
    // Update the max current tech range.
    maxTechLevel = 0;
    for (std::set<unsigned int>::iterator iter = completedTechs.begin(); iter != completedTechs.end(); iter++)
    {
        if (TechConfig::Techs[*iter].techLevel > maxTechLevel)
        {
            maxTechLevel = TechConfig::Techs[*iter].techLevel;
        }
    }

    // Update the min current tech range.
    minTechLevel = std::numeric_limits<int>::max();
    for (std::set<unsigned int>::iterator iter = techsAvailableForResearch.begin(); iter != techsAvailableForResearch.end(); iter++)
    {
        if (TechConfig::Techs[*iter].techLevel < minTechLevel)
        {
            minTechLevel = TechConfig::Techs[*iter].techLevel;
        }
    }

    // Update techs available for research.
    for (unsigned int i = 0; i < TechConfig::Techs.size(); i++)
    {
        // Tech isn't completed or in the researchable list
        if (completedTechs.find(i) == completedTechs.end() && techsAvailableForResearch.find(i) == techsAvailableForResearch.end())
        {
            // If either of the dependency sets are satisfied, the technology is available for research.
            for (unsigned int j = 0; j < TechConfig::Techs[i].dependencies.size(); j++)
            {
                bool dependenciesSatisfied = true;
                for (unsigned int k = 0; k < TechConfig::Techs[i].dependencies[j].size(); k++)
                {
                    if (completedTechs.find(TechConfig::Techs[i].dependencies[j][k]) == completedTechs.end())
                    {
                        dependenciesSatisfied = false;
                        break;
                    }
                }

                if (dependenciesSatisfied)
                {
                    techsAvailableForResearch.insert(i);
                    break;
                }
            }
        }
    }
}