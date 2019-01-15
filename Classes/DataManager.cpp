#include "DataManager.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

DataManager::DataManager()
	: mDefaultStr("default")
{
	loadMainConfig("main_config.json");
	loadSettings();
	loadResources("db/images.json", mData.images);							
	loadResources("db/fonts.json", mData.fonts);							
	loadResources("locale/" + mSettings.locale + ".json", mData.strings);	
	loadResources("db/views.json", mData.views);	

	loadLevels("db/levels.json");
	loadMonsters("objects/monsters.json");

}

DataManager* DataManager::getInstance()
{
	static DataManager manager;
	return &manager;
}

const sData& DataManager::getData() const
{
	return mData;
}

sSettings& DataManager::getSettings()
{
	return mSettings;
}

void DataManager::loadMainConfig(const std::string& aPath)
{
	rapidjson::Document config;
	std::string configContent = cocos2d::FileUtils::getInstance()->getStringFromFile(aPath);
	config.Parse(configContent.c_str());
	if (!config.HasParseError())
	{
		for (auto it = config.MemberBegin(); it != config.MemberEnd(); ++it)
		{
			std::string name = it->name.GetString();
			if (name == "desktop_resolution")
			{
				rapidjson::Value& configResolution = it->value;
				if (configResolution.HasMember("width"))
				{
					mData.desktopResolution.width = static_cast<float>(configResolution["width"].GetInt());
				}
				if (configResolution.HasMember("height"))
				{
					mData.desktopResolution.height = static_cast<float>(configResolution["height"].GetInt());
				}
			}
			else if (name == "design_resolution")
			{
				rapidjson::Value& configResolution = it->value;
				if (configResolution.HasMember("width"))
				{
					mData.designResolution.width = static_cast<float>(configResolution["width"].GetInt());
				}
				if (configResolution.HasMember("height"))
				{
					mData.designResolution.height = static_cast<float>(configResolution["height"].GetInt());
				}
			}
		}
	}
	else
	{
		CCLOG("'%s' parsing error", aPath.c_str());
	}
}

void DataManager::loadSettings()
{
	rapidjson::Document config;
	const std::string path = cocos2d::FileUtils::getInstance()->getWritablePath() + "/settings.json";

	if(!cocos2d::FileUtils::getInstance()->isFileExist(path))
	{
		auto fileUtils = cocos2d::FileUtils::getInstance();
		const std::string settingsFile = fileUtils->getWritablePath() + "settings.json";
		if (!fileUtils->isFileExist(settingsFile))
		{
			const std::string defaultSettings = fileUtils->getStringFromFile("default_settings.json");
			fileUtils->writeStringToFile(defaultSettings, settingsFile);
			CCLOG("'%s' was created", settingsFile.c_str());
		}
	}

	std::string configContent = cocos2d::FileUtils::getInstance()->getStringFromFile(path);
	config.Parse(configContent.c_str());
	if (!config.HasParseError())
	{
		for (auto it = config.MemberBegin(); it != config.MemberEnd(); ++it)
		{
			std::string itName = it->name.GetString();
			if (itName == "locale")
			{
				mSettings.locale = it->value.GetString();
			}
		}
	}
	else
	{
		CCLOG("'%s' parsing error", path.c_str());
	}
}

void DataManager::loadResources(const std::string& aPath, std::map<std::string, std::string>& aContainer)
{
	rapidjson::Document config;
	std::string configContent = cocos2d::FileUtils::getInstance()->getStringFromFile(aPath);
	config.Parse(configContent.c_str());
	if (!config.HasParseError())
	{
		aContainer[mDefaultStr] = mDefaultStr;
		for (auto it = config.MemberBegin(); it != config.MemberEnd(); ++it)
		{
			aContainer[it->name.GetString()] = it->value.GetString();
		}
	}
	else
	{
		CCLOG("'%s' parsing error", aPath.c_str());
	}
}

void DataManager::saveSettings()
{
	rapidjson::Document settings;
	settings.SetObject();
	rapidjson::Value localeObj;
	localeObj.SetString(mSettings.locale.c_str(), settings.GetAllocator());
	settings.AddMember("locale", localeObj, settings.GetAllocator());

	//output
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	settings.Accept(writer);
	const std::string settingsFile = cocos2d::FileUtils::getInstance()->getWritablePath() + "/settings.json";
	cocos2d::FileUtils::getInstance()->writeStringToFile(buffer.GetString(), settingsFile);
}
//
//void DataManager::reloadStrings()
//{
//	mData.strings.clear();
//	loadStrings("locale/" + mSettings.locale + ".json");
//}

const std::string& DataManager::getResourceById(const std::string& aID, std::map<std::string, std::string>& aContainer)
{
	auto resIt = aContainer.find(aID);
	if (resIt != aContainer.end())
	{
		return resIt->second;
	}
	return aContainer[mDefaultStr];
}

void DataManager::loadLevels(const std::string& aPath)
{
	rapidjson::Document levelsDB;
	std::string configContent = cocos2d::FileUtils::getInstance()->getStringFromFile(aPath);
	levelsDB.Parse(configContent.c_str());
	if (!levelsDB.HasParseError())
	{
		for (auto it = levelsDB.MemberBegin(); it != levelsDB.MemberEnd(); ++it)
		{
			rapidjson::Document currentLevelConfig;
			std::string currentLevelConfigContent = cocos2d::FileUtils::getInstance()->getStringFromFile(it->value.GetString());
			currentLevelConfig.Parse(currentLevelConfigContent.c_str());
			if (!currentLevelConfig.HasParseError())
			{
				const std::string levelIDStr = it->name.GetString();
				eLevelID levelID = getLevelIDEnumFromLevelIDString(levelIDStr);
				sLevel& currentLevel = mData.levels[levelID];

				for (auto levelConfigIt = currentLevelConfig.MemberBegin(); levelConfigIt != currentLevelConfig.MemberEnd(); ++levelConfigIt)
				{
					std::string levelAttributeName = levelConfigIt->name.GetString();
					if (levelAttributeName == "background")
					{
						currentLevel.background = levelConfigIt->value.GetString();
					}
				}
			}
			else
			{
				CCLOG("'%s' parsing error", aPath.c_str());
			}
		}
	}
	else
	{
		CCLOG("'%s' parsing error", aPath.c_str());
	}
}

void DataManager::loadMonsters(const std::string& aPath)
{
	rapidjson::Document monsterConfig;
	std::string configContent = cocos2d::FileUtils::getInstance()->getStringFromFile(aPath);
	monsterConfig.Parse(configContent.c_str());
	if (!monsterConfig.HasParseError())
	{
		for (auto monsterIt = monsterConfig.MemberBegin(); monsterIt != monsterConfig.MemberEnd(); ++monsterIt)
		{
			const std::string monsterIDStr = monsterIt->name.GetString();
			eMonsterID monsterID = getMonsterIDEnumFromMonsterIDString(monsterIDStr);
			if (monsterID != eMonsterID::MONSTER_UNKNOWN)
			{
				sMonster& monster = mData.monsters[monsterID];
				rapidjson::Value& monsterAttr = monsterIt->value;
				for (auto monsterAttrIt = monsterAttr.MemberBegin(); monsterAttrIt != monsterAttr.MemberEnd(); ++monsterAttrIt)
				{
					const std::string attrName = monsterAttrIt->name.GetString();
					if (attrName == "sprite")
					{
						monster.spriteFrameName = monsterAttrIt->value.GetString();
					}
					else if (attrName == "name")
					{
						monster.name = monsterAttrIt->value.GetString();
					}
					else if (attrName == "HP")
					{
						monster.hp = static_cast<float>(monsterAttrIt->value.GetDouble());
					}
				}
			}
			else
			{
				CCLOG("'%s' is unknown monster", monsterIDStr.c_str());
			}
		}
	}
	else
	{
		CCLOG("'%s' parsing error", aPath.c_str());
	}
}

const std::string& DataManager::getStringById(const std::string& aID)
{
	return getResourceById(aID, mData.strings);
}

const std::string& DataManager::getFrameNameById(const std::string& aID)
{
	return getResourceById(aID, mData.images);
}

const std::string& DataManager::getFontById(const std::string& aID)
{
	return getResourceById(aID, mData.fonts);
}

const std::string& DataManager::getViewById(const std::string& aID)
{
	return getResourceById(aID, mData.views);
}

eLevelID DataManager::getLevelIDEnumFromLevelIDString(const std::string& aID)
{
	eLevelID result = eLevelID::LEVEL_UNKNOWN;

	if (aID == "LEVEL_WASTELAND")
	{
		result = eLevelID::LEVEL_WASTELAND;
	}
	else if (aID == "LEVEL_CANYON")
	{
		result = eLevelID::LEVEL_CANYON;
	}
	else if (aID == "LEVEL_FOREST")
	{
		result = eLevelID::LEVEL_FOREST;
	}
	else if (aID == "LEVEL_CASTLE")
	{
		result = eLevelID::LEVEL_CASTLE;
	}

	return result;
}

eMonsterID DataManager::getMonsterIDEnumFromMonsterIDString(const std::string& aID)
{
	eMonsterID result = eMonsterID::MONSTER_UNKNOWN;

	if (aID == "SKELETON")
	{
		result = eMonsterID::MONSTER_SKELETON;
	}

	return result;
}

//std::string DataManager::getLevelIDStringFromLevelIDEnum()
//{
//
//}