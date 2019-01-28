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
bool DataManager::loadViewFromJson(cocos2d::Node* aParent, const std::string& aJson)
{
	return mViewBuilder.loadFromJson(aParent, aJson);
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
				const std::string levelID = it->name.GetString();
				sLevel& currentLevel = mData.levels[levelID];

				for (auto levelAttrIt = currentLevelConfig.MemberBegin(); levelAttrIt != currentLevelConfig.MemberEnd(); ++levelAttrIt)
				{
					std::string levelAttrName = levelAttrIt->name.GetString();
					if (levelAttrName == "background")
					{
						currentLevel.background = levelAttrIt->value.GetString();
					}
					else if (levelAttrName == "spawnpoint_x")
					{
						currentLevel.spawnPoint.x = static_cast<float>(levelAttrIt->value.GetDouble());
					}
					else if (levelAttrName == "spawnpoint_y")
					{
						currentLevel.spawnPoint.y = static_cast<float>(levelAttrIt->value.GetDouble());
					}
					else if (levelAttrName == "monsters")
					{
						rapidjson::Value& monsterArray = levelAttrIt->value;
						for (auto monsterIt = monsterArray.Begin(); monsterIt != monsterArray.End(); ++monsterIt)
						{
							currentLevel.monsters.emplace_back(monsterIt->GetString());
						}
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
		const std::string defaultMonsterID = "DEFAULT_MONSTER";
		bool isDefaultMonsterExists = false;

		for (auto monsterIt = monsterConfig.MemberBegin(); monsterIt != monsterConfig.MemberEnd(); ++monsterIt)
		{
			const std::string monsterID = monsterIt->name.GetString();
			sMonster& monster = mData.monsters[monsterID];

			if (isDefaultMonsterExists)
			{
				monster = mData.monsters[defaultMonsterID];
				CCLOG("Set current monster as default");
			}
			else
			{
				isDefaultMonsterExists = (monsterID == defaultMonsterID);
				CCLOG("Check default monster");
			}

			rapidjson::Value& monsterAttr = monsterIt->value;
			for (auto monsterAttrIt = monsterAttr.MemberBegin(); monsterAttrIt != monsterAttr.MemberEnd(); ++monsterAttrIt)
			{
				const std::string attrName = monsterAttrIt->name.GetString();
				if (attrName == "sprite")
				{
					monster.spriteFrameNameID = monsterAttrIt->value.GetString();
				}
				else if (attrName == "name")
				{
					monster.nameSTID = monsterAttrIt->value.GetString();
				}
				else if (attrName == "HP")
				{
					monster.hp = static_cast<float>(monsterAttrIt->value.GetDouble());
				}
				else if (attrName == "anchor_x")
				{
					monster.anchor.x = static_cast<float>(monsterAttrIt->value.GetDouble());
				}
				else if (attrName == "anchor_y")
				{
					monster.anchor.y = static_cast<float>(monsterAttrIt->value.GetDouble());
				}
				else if (attrName == "phys_resist")
				{
					monster.resistance.physical = monsterAttrIt->value.GetInt();
				}
				else if (attrName == "mag_resist")
				{
					monster.resistance.magical = monsterAttrIt->value.GetInt();
				}
				else if (attrName == "appearing_time")
				{
					monster.appearingTime = static_cast<float>(monsterAttrIt->value.GetDouble());
				}
				else if (attrName == "disappearing_time")
				{
					monster.disappearingTime = static_cast<float>(monsterAttrIt->value.GetDouble());
				}
				else if (attrName == "touch_action_time")
				{
					monster.touchActionTime = static_cast<float>(monsterAttrIt->value.GetDouble());
				}
			}
		}

		if (isDefaultMonsterExists)
		{
			mData.monsters.erase(defaultMonsterID);
			CCLOG("default monster was erased");
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