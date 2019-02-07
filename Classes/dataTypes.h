#ifndef DATATYPES_H
#define DATATYPES_H

#include "cocos2d.h"
#include <string>
#include <map>

struct sDamage
{
	int physical;
	int magical;

	sDamage()
		: physical(0)
		, magical(0)
	{
	}
};

struct sItem
{
	std::string frameNameID;
	sDamage damage;
};

struct sResistance
{
	int physical;
	int magical;

	sResistance()
		: physical(0)
		, magical(0)
	{
	}
};

struct sMonster
{
	std::string spriteFrameNameID;
	std::string nameSTID;
	cocos2d::Vec2 anchor;
	float appearingTime;
	float disappearingTime;
	float touchActionTime;
	float healingTime;

	float hp;
	sResistance resistance;

	unsigned int goldReward;

	sMonster()
		: anchor(cocos2d::Vec2::ANCHOR_MIDDLE)
		, appearingTime(0.0f)
		, disappearingTime(0.0f)
		, touchActionTime(0.0f)
		, hp(0.0f)
		, healingTime(0.0f)
		, goldReward(0)
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
	std::map<std::string, sItem> items;
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

