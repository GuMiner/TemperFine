#include "Logger.h"
#include "PhysicsConfig.h"

int PhysicsConfig::PhysicsThreadDelay;
float PhysicsConfig::ViewForwardsSpeed;
float PhysicsConfig::ViewSidewaysSpeed;

float PhysicsConfig::ViewRotateUpFactor;
float PhysicsConfig::ViewRotateAroundFactor;

bool PhysicsConfig::LoadConfigValues(std::vector<std::string>& configFileLines)
{
    return (ReadInt(configFileLines, PhysicsThreadDelay, "Error decoding the physics thread delay!") &&
            ReadFloat(configFileLines, ViewForwardsSpeed, "Error reading in the view forwards speed!") &&
            ReadFloat(configFileLines, ViewSidewaysSpeed, "Error reading in the view sideways speed!") &&
            ReadFloat(configFileLines, ViewRotateUpFactor, "Error reading in the view rotate up factor!") &&
            ReadFloat(configFileLines, ViewRotateAroundFactor, "Error reading in the view rotate around factor!"));
}

void PhysicsConfig::WriteConfigValues()
{
	WriteInt("PhysicsThreadDelay", PhysicsThreadDelay);
	WriteFloat("ViewForwardsSpeed", ViewForwardsSpeed);
	WriteFloat("ViewSidewaysSpeed", ViewSidewaysSpeed);

	WriteFloat("ViewRotateUpFactor", ViewRotateUpFactor);
	WriteFloat("ViewRotateAroundFactor", ViewRotateAroundFactor);
}

PhysicsConfig::PhysicsConfig(const char* configName)
	: ConfigManager(configName)
{
}
