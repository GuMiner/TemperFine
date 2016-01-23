#pragma once
#include <vector>
#include "BodyInfo.h"
#include "ConfigManager.h"
#include "ModelManager.h"

class BodyConfig : public ConfigManager
{
    ModelManager* modelManager;

	virtual bool LoadConfigValues(std::vector<std::string>& lines);
	virtual void WriteConfigValues();
public:
    static std::vector<BodyType> Bodies;

	BodyConfig(ModelManager* modelManager, const char* configName);
};

