#pragma once
#include <vector>
#include "TurretInfo.h"
#include "ConfigManager.h"
#include "ModelManager.h"

class TurretConfig : public ConfigManager
{
    ModelManager* modelManager;

	virtual bool LoadConfigValues(std::vector<std::string>& lines);
	virtual void WriteConfigValues();
public:
    static std::vector<TurretType> Turrets;

	TurretConfig(ModelManager* modelManager, const char* configName);
};

