#pragma once
#include "ConfigManager.h"

class PhysicsConfig : public ConfigManager
{
	virtual bool LoadConfigValues(std::vector<std::string>& lines);
	virtual void WriteConfigValues();
public:
	static int PhysicsThreadDelay;
	static float ViewForwardsSpeed;
	static float ViewSidewaysSpeed;

	PhysicsConfig(const char* configName);
};

