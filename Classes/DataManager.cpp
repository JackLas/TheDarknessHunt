#include "DataManager.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

DataManager::DataManager()
{
	loadMainConfig("main_config.json");
	loadSettings();
	loadImages("db/images.json");
	loadFonts("db/fonts.json");
	loadStrings("locale/" + mSettings.locale + ".json");
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

void DataManager::loadImages(const std::string& aPath)
{
	rapidjson::Document config;
	std::string configContent = cocos2d::FileUtils::getInstance()->getStringFromFile(aPath);
	config.Parse(configContent.c_str());
	if (!config.HasParseError())
	{
		auto& images = mData.images;
		for (auto it = config.MemberBegin(); it != config.MemberEnd(); ++it)
		{
			images[it->name.GetString()] = it->value.GetString();
		}
	}
	else
	{
		CCLOG("'%s' parsing error", aPath.c_str());
	}
}

void DataManager::loadFonts(const std::string& aPath)
{
	rapidjson::Document config;
	std::string configContent = cocos2d::FileUtils::getInstance()->getStringFromFile(aPath);
	config.Parse(configContent.c_str());
	if (!config.HasParseError())
	{
		auto& fonts = mData.fonts;
		for (auto it = config.MemberBegin(); it != config.MemberEnd(); ++it)
		{
			fonts[it->name.GetString()] = it->value.GetString();
		}
	}
	else
	{
		CCLOG("'%s' parsing error", aPath.c_str());
	}
}

void DataManager::loadStrings(const std::string& aPath)
{
	rapidjson::Document config;
	std::string configContent = cocos2d::FileUtils::getInstance()->getStringFromFile(aPath);
	config.Parse(configContent.c_str());
	if (!config.HasParseError())
	{
		auto& strings = mData.strings;
		for (auto it = config.MemberBegin(); it != config.MemberEnd(); ++it)
		{
			strings[it->name.GetString()] = it->value.GetString();
		}
	}
	else
	{
		CCLOG("'%s' parsing error, locale set 'en'", aPath.c_str());
		loadStrings("locale/en.json");
	}
}

//void DataManager::saveSettings()
//{
//	rapidjson::Document settings;
//	settings.SetObject();
//	rapidjson::Value localeObj;
//	localeObj.SetString(mSettings.locale.c_str(), settings.GetAllocator());
//	settings.AddMember("locale", localeObj, settings.GetAllocator());
//
//	//output
//	rapidjson::StringBuffer buffer;
//	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
//	settings.Accept(writer);
//	const std::string settingsFile = cocos2d::FileUtils::getInstance()->getWritablePath() + "/settings.json";
//	cocos2d::FileUtils::getInstance()->writeStringToFile(buffer.GetString(), settingsFile);
//}
//
//void DataManager::reloadStrings()
//{
//	mData.strings.clear();
//	loadStrings("locale/" + mSettings.locale + ".json");
//}