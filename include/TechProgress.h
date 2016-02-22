#pragma once
#include <set>

// Current technology progress of the given player.
class TechProgress
{
public:

    // Sets an empty tech progress with a user who just has the basic tank.
    TechProgress();

    // Completes a tech.
    void CompleteTech(unsigned int tech);

    // True if the tech exists, false otherwise.
    bool HasTech(const char* techInternalName);
    bool HasTech(unsigned int techId);

private:
    std::set<unsigned int> completedTechs;
    std::set<unsigned int> techsAvailableForResearch;

    int minTechLevel; // Min level of techs to acquire.
    int maxTechLevel; // Max level of acquired techs.

    // Updates the min/max current tech range and techs available for research.
    void UpdateTechs();
};

