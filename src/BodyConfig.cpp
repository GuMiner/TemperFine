#include <sstream>
#include "Logger.h"
#include "BodyConfig.h"
#include "StringUtils.h"

std::vector<BodyType> BodyConfig::Bodies;

bool BodyConfig::LoadConfigValues(std::vector<std::string>& configFileLines)
{
    // Move from the ConfigVersion line to actual data.
    ++lineCounter;

    int linesPerBody = 2;
    while (lineCounter + linesPerBody <= (int)configFileLines.size())
    {
        // Load in the turret name.
        BodyType bodyType;
        bodyType.name = std::string(configFileLines[lineCounter]);

        std::stringstream loadingMsg;
        loadingMsg << "Loading model \"" << bodyType.name << "\".";
        Logger::Log(loadingMsg.str().c_str());

        // Load in all the minor bodyType details, after breaking apart the single detail line.
        std::vector<std::string> modelLines;

        StringUtils::Split(configFileLines[lineCounter + 1], StringUtils::Space, true, modelLines);
        if (modelLines.size() != 3)
        {
            Logger::LogError("Expected 3 elements for a model body configuration line.");
            return false;
        }

        int maxTurrets;
        if (!StringUtils::ParseIntFromString(modelLines[1], maxTurrets) ||
            !StringUtils::ParseFloatFromString(modelLines[2], bodyType.scale))
        {
            Logger::LogError("Error parsing the body max turrets and scale offsets.");
            return false;
        }

        bodyType.maxTurrets = (unsigned int)maxTurrets;

        // Finally, given the model name, load in the turret model.
        std::stringstream bodyFileName;
        bodyFileName << "models/bodies/" << modelLines[0];
        bodyType.bodyModelId = modelManager->LoadModel(bodyFileName.str().c_str());
        if (bodyType.bodyModelId == 0)
        {
            Logger::Log("Body model loading failed!");
            return false;
        }

        // Save the loaded turret information.
        Bodies.push_back(bodyType);
        lineCounter += linesPerBody;
    }

    return true;
}

void BodyConfig::WriteConfigValues()
{
	// Cannot write to unit-type configuration files.
}

BodyConfig::BodyConfig(ModelManager *modelManager, const char* configName)
	: ConfigManager(configName), modelManager(modelManager)
{
}
