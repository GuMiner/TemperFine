#include <sstream>
#include "Logger.h"
#include "ArmorConfig.h"
#include "StringUtils.h"

std::vector<ArmorType> ArmorConfig::Armors;

bool ArmorConfig::LoadConfigValues(std::vector<std::string>& configFileLines)
{
    // Move from the ConfigVersion line to actual data.
    ++lineCounter;

    int linesPerArmor = 4;
    while (lineCounter + linesPerArmor <= (int)configFileLines.size())
    {
        // Load in the armor name.
        ArmorType armorType;
        armorType.name = std::string(configFileLines[lineCounter]);

        std::stringstream loadingMsg;
        loadingMsg << "Loading model \"" << armorType.name << "\".";
        Logger::Log(loadingMsg.str().c_str());

        // Load in all the minor armor details, after breaking apart the three detail lines.
        std::vector<std::string> modelLines;
        std::vector<std::string> statLines;
        std::vector<std::string> penaltyLines;

        StringUtils::Split(configFileLines[lineCounter + 1], StringUtils::Space, true, modelLines);
        if (modelLines.size() != 8)
        {
            Logger::LogError("Expected 8 elements for a model armor configuration line.");
            return false;
        }

        StringUtils::Split(configFileLines[lineCounter + 2], StringUtils::Space, true, statLines);
        if (statLines.size() != 3)
        {
            Logger::LogError("Expected 3 elements for a stat armor configuration line.");
            return false;
        }

        StringUtils::Split(configFileLines[lineCounter + 3], StringUtils::Space, true, penaltyLines);
        if (penaltyLines.size() != 3)
        {
            Logger::LogError("Expected 3 elements for a penalty armor configuration line.");
            return false;
        }

        if (!StringUtils::ParseFloatFromString(modelLines[1], armorType.translationOffset.x) ||
            !StringUtils::ParseFloatFromString(modelLines[2], armorType.translationOffset.y) ||
            !StringUtils::ParseFloatFromString(modelLines[3], armorType.translationOffset.z) ||
            !StringUtils::ParseFloatFromString(modelLines[4], armorType.rotationOffset.x) ||
            !StringUtils::ParseFloatFromString(modelLines[5], armorType.rotationOffset.y) ||
            !StringUtils::ParseFloatFromString(modelLines[6], armorType.rotationOffset.z) ||
            !StringUtils::ParseFloatFromString(modelLines[7], armorType.rotationOffset.w))
        {
            Logger::LogError("Error parsing the armor translation and rotation offsets.");
            return false;
        }


        if (!StringUtils::ParseFloatFromString(statLines[0], armorType.maxMachineGun) ||
            !StringUtils::ParseFloatFromString(statLines[1], armorType.maxRocket) ||
            !StringUtils::ParseFloatFromString(statLines[2], armorType.maxCannon))
        {
            Logger::LogError("Error parsing the maximum armor duration stats.");
            return false;
        }

        if (!StringUtils::ParseFloatFromString(penaltyLines[0], armorType.maxMachineGunPenalty) ||
            !StringUtils::ParseFloatFromString(penaltyLines[1], armorType.maxRocketPenalty) ||
            !StringUtils::ParseFloatFromString(penaltyLines[2], armorType.maxCannonPenalty))
        {
            Logger::LogError("Error parsing the armor penalty stats.");
            return false;
        }

        // Finally, given the model name, load in the armor model.
        std::stringstream armorFileName;
        armorFileName << "models/armors/" << modelLines[0];
        armorType.armorModelId = modelManager->LoadModel(armorFileName.str().c_str());
        if (armorType.armorModelId == 0)
        {
            Logger::Log("Armor model loading failed!");
            return false;
        }

        // Save the loaded armor information.
        Armors.push_back(armorType);
        lineCounter += linesPerArmor;
    }

    return true;
}

void ArmorConfig::WriteConfigValues()
{
	// Cannot write to unit-type configuration files.
}

ArmorConfig::ArmorConfig(ModelManager *modelManager, const char* configName)
	: ConfigManager(configName), modelManager(modelManager)
{
}
