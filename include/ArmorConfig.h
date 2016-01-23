#pragma once
#include <vector>
#include "ArmorInfo.h"
#include "ConfigManager.h"
#include "ModelManager.h"

class ArmorConfig : public ConfigManager
{
    ModelManager* modelManager;

	virtual bool LoadConfigValues(std::vector<std::string>& lines);
	virtual void WriteConfigValues();
public:
    static std::vector<ArmorType> Armors;

	ArmorConfig(ModelManager* modelManager, const char* configName);
};

