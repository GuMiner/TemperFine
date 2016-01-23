#pragma once
#include <vector>
#include "TechInfo.h"
#include "ConfigManager.h"
#include "ModelManager.h"

class TechConfig : public ConfigManager
{
	virtual bool LoadConfigValues(std::vector<std::string>& lines);
	virtual void WriteConfigValues();
public:
    static std::vector<Tech> Techs;

	TechConfig(const char* configName);
};

