#pragma once
#include <SFML\Graphics.hpp>
#include <set>
#include <string>
#include <vector>
#include "vmath.hpp"

// Represents a technology
struct Tech
{
    // Vector of vector of required dependencies.
    //  If a singe set of the dependent technologies have been researched,
    //   this technology can then be researched.
    std::vector<std::vector<unsigned int>> dependencies;

    // Display name of the technology. Varies based on the researched dependencies.
    std::vector<std::string> names;

    // Display image of the technology.
    sf::Image techImage;

    // Time required to research this technology.
    int researchTimeSeconds;

    // Resources required to research this technology.
    float researchResources;

    // Internal name, used for dependency formation. Not intended for other usages.
    std::string internalName;

    // The effective dependency level of this technology.
    int techLevel;
};

struct TechProgress
{
    std::set<unsigned int> completedTechs;
    std::set<unsigned int> techsAvailableForResearch;


    int GetMinTechLevel()
    {
        return 0;
    }

    int GetMaxTechLevel()
    {
        return 0;
    }
};
