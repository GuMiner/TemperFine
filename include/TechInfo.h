#pragma once
#include <SFML\Graphics.hpp>
#include <string>
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
};
