#ifndef DATATYPES_H
#define DATATYPES_H

#include "cocos2d.h"
#include <string>
#include <map>

struct sMonster
{
	std::string spriteFrameNameID;
	std::string nameSTID;
	float hp;

	sMonster()
		: hp(0.0f)
	{
	}
};

struct sLevel
{
	std::string background;
	cocos2d::Vec2 spawnPoint;
	std::vector<std::string> monsters;
};

struct sData
{
	std::map<std::string, std::string> images;
	std::map<std::string, std::string> fonts;
	std::map<std::string, std::string> strings;
	std::map<std::string, std::string> views;
	std::map<std::string, sLevel> levels;
	std::map<std::string, sMonster> monsters;
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

