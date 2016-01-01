#include "Logger.h"
#include "PhysicsConfig.h"

int PhysicsConfig::PhysicsThreadDelay;
float PhysicsConfig::ViewForwardsSpeed;
float PhysicsConfig::ViewSidewaysSpeed;

bool PhysicsConfig::LoadConfigValues(std::vector<std::string>& configFileLines)
{
    return (ReadInt(configFileLines, PhysicsThreadDelay, "Error decoding the physics thread delay!") &&
            ReadFloat(configFileLines, ViewForwardsSpeed, "Error reading in the view forwards speed!") &&
            ReadFloat(configFileLines, ViewSidewaysSpeed, "Error reading in the view sideways speed!"));
}

void PhysicsConfig::WriteConfigValues()
{
	WriteInt("PhysicsThreadDelay", PhysicsThreadDelay);
	WriteFloat("ViewForwardsSpeed", ViewForwardsSpeed);
	WriteFloat("ViewSidewaysSpeed", ViewSidewaysSpeed);
}

PhysicsConfig::PhysicsConfig(const char* configName)
	: ConfigManager(configName)
{
}
