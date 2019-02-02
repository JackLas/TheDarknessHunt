#include "Player.h"
#include <string>
#include <chrono>
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/prettywriter.h"

Player::Player()
	: mIsNeedSaving(false)
	, mTimeBetweenGameLaunchings(0)
	, mGold(0)
{
	mTimeFile = cocos2d::FileUtils::getInstance()->getWritablePath() + "/data.time";
	mSaveFile = cocos2d::FileUtils::getInstance()->getWritablePath() + "/data.save";
	mHashFile = cocos2d::FileUtils::getInstance()->getWritablePath() + "/data.hash";

	loadData();

	const float timeSavingInterval = 1.0f;
	auto scheduler = cocos2d::Director::getInstance()->getScheduler();
	scheduler->schedule([this](float aDt)
	{
		typedef std::chrono::system_clock clock;
		cocos2d::FileUtils::getInstance()->writeStringToFile(
			std::to_string(clock::to_time_t(clock::now())), mTimeFile);

	}, this, timeSavingInterval, CC_REPEAT_FOREVER, 0.0f, false, "timeSaver");

	const float dataSavingInterval = 5.0f;
	scheduler->schedule([this](float aDt)
	{
		if (mIsNeedSaving)
		{
			saveData();
			mIsNeedSaving = false;
		}
	}, this, dataSavingInterval, CC_REPEAT_FOREVER, 0.0f, false, "dataSaver");
}

Player::~Player()
{
}

Player* Player::getInstance()
{
	static Player player;
	return &player;
}

void Player::loadData()
{
	typedef std::chrono::system_clock clock;
	if (cocos2d::FileUtils::getInstance()->isFileExist(mTimeFile))
	{
		std::string timeData = cocos2d::FileUtils::getInstance()->getStringFromFile(mTimeFile);
		mTimeBetweenGameLaunchings = clock::to_time_t(clock::now()) - std::stoll(timeData);
	}

	if (cocos2d::FileUtils::getInstance()->isFileExist(mSaveFile) &&
		cocos2d::FileUtils::getInstance()->isFileExist(mHashFile))
	{
		std::string	currentSaveHash = cocos2d::utils::getFileMD5Hash(mSaveFile);
		std::string realSaveHash = cocos2d::FileUtils::getInstance()->getStringFromFile(mHashFile);
		if (currentSaveHash == realSaveHash)
		{
			rapidjson::Document save;
			std::string configContent = cocos2d::FileUtils::getInstance()->getStringFromFile(mSaveFile);
			save.Parse(configContent.c_str());
			if (!save.HasParseError())
			{
				for (auto it = save.MemberBegin(); it != save.MemberEnd(); ++it)
				{
					std::string name = it->name.GetString();
					if (name == "kills")
					{
						rapidjson::Value& levels = it->value;
						for (auto level = levels.MemberBegin(); level != levels.MemberEnd(); ++level)
						{
							std::string levelName = level->name.GetString();
							mKills[levelName] = static_cast<unsigned int>(level->value.GetInt());
						}
					}
					else if (name == "gold")
					{
						mGold = static_cast<unsigned int>(it->value.GetInt());
					}
				}
			}
			else
			{
				CCLOG("save file parsing error");
			}
		}
	}
}

void Player::saveData()
{
	rapidjson::Document saveData;
	saveData.SetObject();
	rapidjson::Value killsObject;
	killsObject.SetObject();
	for (const auto& kills : mKills)
	{
		rapidjson::Value killsValue(kills.second);
		char buffer[64];
		int len = sprintf(buffer, "%s", kills.first.c_str());
		rapidjson::Value killsName;
		killsName.SetString(buffer, len, saveData.GetAllocator());
		killsObject.AddMember(killsName, killsValue, saveData.GetAllocator());
	}
	saveData.AddMember("kills", killsObject, saveData.GetAllocator());
	rapidjson::Value gold(mGold);
	saveData.AddMember("gold", gold, saveData.GetAllocator());

	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
	saveData.Accept(writer);
	cocos2d::FileUtils::getInstance()->writeStringToFile(buffer.GetString(), mSaveFile);

	std::string saveHash = cocos2d::utils::getFileMD5Hash(mSaveFile);
	cocos2d::FileUtils::getInstance()->writeStringToFile(saveHash, mHashFile);
}

void Player::resetProgress()
{
	mGold = 0;
	mKills.clear();
	cocos2d::FileUtils::getInstance()->removeFile(mSaveFile);
	cocos2d::FileUtils::getInstance()->removeFile(mHashFile);
	mIsNeedSaving = false;
}

const std::time_t& Player::getTimeBetweenGameLaunchings() const 
{
	return mTimeBetweenGameLaunchings;
}

void Player::setMapPosition(const cocos2d::Vec2& aPosition)
{
	mMapPosition = aPosition;
}

const cocos2d::Vec2& Player::getMapPosition() const 
{
	return mMapPosition;
}

void Player::addKill(const std::string& aLevelID, const unsigned int aAmount)
{
	const auto levelIt = mKills.find(aLevelID);
	if (levelIt != mKills.end())
	{
		levelIt->second += aAmount;
	}
	else
	{
		mKills[aLevelID] = aAmount;
	}
	mIsNeedSaving = true;
}

const unsigned int& Player::getKills(const std::string& aLevelID)
{
	const auto levelIt = mKills.find(aLevelID);
	if (levelIt != mKills.end())
	{
		return levelIt->second;
	}
	else
	{
		return (mKills[aLevelID] = 0);
	}
}

void Player::addGold(const unsigned int aAmount)
{
	mGold += aAmount;
	mIsNeedSaving = true;
}

const unsigned int& Player::getGold()
{
	return mGold;
}
