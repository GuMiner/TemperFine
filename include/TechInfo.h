#pragma once
#include <SFML\Graphics.hpp>
#include <string>
#include <vector>
#include "TechConfig.h"
#include "Vec.h"

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

    // Internal name, used for dependency formation and for conditional operations (does user have 'sensors', etc).
    std::string internalName;

    // The effective dependency level of this technology.
    int techLevel;
};