#include "Player.h"
#include "cocos2d.h"
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

const std::time_t& Player::getTimeBetweenGameLaunchings()
{
	return mTimeBetweenGameLaunchings;
}

Player* Player::getInstance()
{
	static Player player;
	return &player;
}
