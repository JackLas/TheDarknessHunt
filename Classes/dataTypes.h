#ifndef DATATYPES_H
#define DATATYPES_H

#include "cocos2d.h"
#include <string>
#include <map>

enum class eLevelID
{
	LEVEL_UNKNOWN = 0,
	LEVEL_WASTELAND,
	LEVEL_CANYON,
	LEVEL_FOREST,
	LEVEL_CASTLE,

	LEVEL_COUNT
};

struct sLevel
{
	std::string background;
};

struct sData
{
	std::map<std::string, std::string> images;
	std::map<std::string, std::string> fonts;
	std::map<std::string, std::string> strings;
	std::map<std::string, std::string> views;
	std::map<eLevelID, sLevel> levels;
	cocos2d::Size designResolution;
	cocos2d::Size desktopResolution;
};

struct sSettings
{
	std::string locale;

	sSettings()
		: locale("en")
	{
	}
};

#endif //DATATYPES_H

