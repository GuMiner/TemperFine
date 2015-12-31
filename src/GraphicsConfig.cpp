#include "Logger.h"
#include "GraphicsConfig.h"

bool GraphicsConfig::IsFullscreen;
int GraphicsConfig::ScreenWidth;
int GraphicsConfig::ScreenHeight;

int GraphicsConfig::TextImageSize;

bool GraphicsConfig::LoadConfigValues(std::vector<std::string>& configFileLines)
{
    return (ReadBool(configFileLines, IsFullscreen, "Error decoding the fullscreen toggle!") &&
            ReadInt(configFileLines, ScreenWidth, "Error reading in the screen width!") &&
            ReadInt(configFileLines, ScreenHeight, "Error reading in the screen height!") &&
            ReadInt(configFileLines, TextImageSize, "Error reading in the text image size!"));
}

void GraphicsConfig::WriteConfigValues()
{
	WriteBool("FullScreen", IsFullscreen);
	WriteInt("ScreenWidth", ScreenWidth);
	WriteInt("ScreenHeight", ScreenHeight);

	WriteInt("TextImageSize", TextImageSize);
}

GraphicsConfig::GraphicsConfig(const char* configName)
	: ConfigManager(configName)
{
}
