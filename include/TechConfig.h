#pragma once
#include <vector>
#include "TechInfo.h"
#include "ConfigManager.h"
#include "ModelManager.h"

class TechConfig : public ConfigManager
{
private:
    const char* LEVEL_ZERO = "none";

    virtual bool LoadConfigValues(std::vector<std::string>& lines);
    virtual void WriteConfigValues();

    // Combines identical technologies that are reachable via the same path into a single tech.
    void CombineIdenticalTechs();

    // Crawls down the tree to assign a tech level to every tech.
    void AssignTechLevels();

public:
    static std::vector<Tech> Techs;
    static sf::Image NoCurrentTechImage;

    TechConfig(const char* configName);
};

