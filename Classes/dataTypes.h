#ifndef DATATYPES_H
#define DATATYPES_H

#include "cocos2d.h"
#include <string>
#include <map>

struct sTavernConfig
{
	std::vector<std::string> welcomeSTIDs;
	std::vector<std::string> hireSTIDs;
	std::vector<std::string> noHireSTIDs;
	int baseHirePrice;
	float messageAppearingTime;
	float messageShowingTime;

	sTavernConfig()
		: baseHirePrice(0)
		, messageAppearingTime(0.0f)
		, messageShowingTime(0.0f)
	{
	}
};

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

sDamage& operator+=(sDamage& aLeft, const sDamage& aRight);
sDamage& operator-=(sDamage& aLeft, const sDamage& aRight);

struct sItem
{
	std::string frameNameID;
	std::string name;
	sDamage damage;
	int price;

	sItem()
		: price(0)
	{

	}
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
	int hirePriceMultiplier;
	int additionalTeammates;

	sLevel()
		: hirePriceMultiplier(1)
		, additionalTeammates(0)
	{
	}
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
	sTavernConfig tavernData;
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

