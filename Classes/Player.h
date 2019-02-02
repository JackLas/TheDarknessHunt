#ifndef PLAYER_H
#define PLAYER_H

#include "cocos2d.h"
#include <ctime>

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

class Player
{
private:
	std::string mTimeFile;
	std::string mSaveFile;
	std::string mHashFile;

	bool mIsNeedSaving;

	std::time_t mTimeBetweenGameLaunchings;
	cocos2d::Vec2 mMapPosition;
	std::map<std::string, unsigned int> mKills; 
	unsigned int mGold;
	sDamage mDamage;

	Player();
	void loadData();

public:
	static Player* getInstance();
	~Player();

	void saveData();
	void resetProgress();

	const std::time_t& getTimeBetweenGameLaunchings() const;
	void setMapPosition(const cocos2d::Vec2& aPosition);
	const cocos2d::Vec2& getMapPosition() const;
	void addKill(const std::string& aLevelID, const unsigned int aAmount = 1);
	const unsigned int& getKills(const std::string& aLevelID);
	void addGold(const unsigned int aAmount);
	const unsigned int& getGold();
};

#define PLAYER Player::getInstance()

#endif //PLAYER_H
