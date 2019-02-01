#include "Player.h"
#include <string>
#include <chrono>

Player::Player()
	: mTimeBetweenGameLaunchings(0)
{
	typedef std::chrono::system_clock clock;

	std::string timeFilePath = cocos2d::FileUtils::getInstance()->getWritablePath() + "/data.time";
	if (cocos2d::FileUtils::getInstance()->isFileExist(timeFilePath))
	{
		std::string timeData = cocos2d::FileUtils::getInstance()->getStringFromFile(timeFilePath);
		mTimeBetweenGameLaunchings = clock::to_time_t(clock::now()) - std::stoll(timeData);
	}

	const float timeSavingInterval = 1.0f;
	auto scheduler = cocos2d::Director::getInstance()->getScheduler();
	scheduler->schedule([](float aDt)
	{
		std::string file = cocos2d::FileUtils::getInstance()->getWritablePath() + "/data.time";
		cocos2d::FileUtils::getInstance()->writeStringToFile(
			std::to_string(clock::to_time_t(clock::now())), file);
	}, this, timeSavingInterval, CC_REPEAT_FOREVER, 0.0f, false, "timesaver");
}

Player::~Player()
{
}

Player* Player::getInstance()
{
	static Player player;
	return &player;
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
