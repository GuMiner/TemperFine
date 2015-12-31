#pragma once
#include "ConfigManager.h"

class GraphicsConfig : public ConfigManager
{
	virtual bool LoadConfigValues(std::vector<std::string>& lines);
	virtual void WriteConfigValues();
public:
	static bool IsFullscreen;
	static int ScreenWidth;
	static int ScreenHeight;

	static int TextImageSize;

	GraphicsConfig(const char* configName);
};

