#ifndef PLAYER_H
#define PLAYER_H

#include "cocos2d.h"
#include <ctime>

class Player
{
private:
	std::time_t mTimeBetweenGameLaunchings;
	cocos2d::Vec2 mMapPosition;
	std::map<std::string, unsigned int> mKills; 

	Player();
public:
	static Player* getInstance();
	~Player();

	const std::time_t& getTimeBetweenGameLaunchings() const;
	void setMapPosition(const cocos2d::Vec2& aPosition);
	const cocos2d::Vec2& getMapPosition() const;
	void addKill(const std::string& aLevelID, const unsigned int aAmount = 1);
	const unsigned int& getKills(const std::string& aLevelID);
};

#define PLAYER Player::getInstance()

#endif //PLAYER_H
