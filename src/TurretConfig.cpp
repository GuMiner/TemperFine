#include <sstream>
#include "Logger.h"
#include "TurretConfig.h"
#include "StringUtils.h"

std::vector<TurretType> TurretConfig::Turrets;

bool TurretConfig::LoadConfigValues(std::vector<std::string>& configFileLines)
{
    // Move from the ConfigVersion line to actual data.
    ++lineCounter;

    int linesPerTurret = 2;
    while (lineCounter + linesPerTurret <= (int)configFileLines.size())
    {
        // Load in the turret name.
        TurretType turretType;
        turretType.name = std::string(configFileLines[lineCounter]);

        std::stringstream loadingMsg;
        loadingMsg << "Loading model \"" << turretType.name << "\".";
        Logger::Log(loadingMsg.str().c_str());

        // Load in all the minor turretType details, after breaking apart the single detail line.
        std::vector<std::string> modelLines;

        StringUtils::Split(configFileLines[lineCounter + 1], StringUtils::Space, true, modelLines);
        if (modelLines.size() != 8)
        {
            Logger::LogError("Expected 8 elements for a model turret configuration line.");
            return false;
        }

        if (!StringUtils::ParseFloatFromString(modelLines[1], turretType.translationOffset.x) ||
            !StringUtils::ParseFloatFromString(modelLines[2], turretType.translationOffset.y) ||
            !StringUtils::ParseFloatFromString(modelLines[3], turretType.translationOffset.z) ||
            !StringUtils::ParseFloatFromString(modelLines[4], turretType.rotationOffset.x) ||
            !StringUtils::ParseFloatFromString(modelLines[5], turretType.rotationOffset.y) ||
            !StringUtils::ParseFloatFromString(modelLines[6], turretType.rotationOffset.z) ||
            !StringUtils::ParseFloatFromString(modelLines[7], turretType.rotationOffset.w))
        {
            Logger::LogError("Error parsing the turret translation and rotation offsets.");
            return false;
        }

        // Finally, given the model name, load in the turret model.
        std::stringstream turretFileName;
        turretFileName << "models/turrets/" << modelLines[0];
        turretType.turretModelId = modelManager->LoadModel(turretFileName.str().c_str());
        if (turretType.turretModelId == 0)
        {
            Logger::Log("Turret model loading failed!");
            return false;
        }

        // Save the loaded turret information.
        Turrets.push_back(turretType);
        lineCounter += linesPerTurret;
    }

    return true;
}

void TurretConfig::WriteConfigValues()
{
	// Cannot write to unit-type configuration files.
}

TurretConfig::TurretConfig(ModelManager *modelManager, const char* configName)
	: ConfigManager(configName), modelManager(modelManager)
{
}
