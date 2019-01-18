#ifndef PLAYER_H
#define PLAYER_H

#include "cocos2d.h"
#include <ctime>

class Player
{
private:
	std::time_t mTimeBetweenGameLaunchings;
	cocos2d::Vec2 mMapPosition;

	Player();
public:
	static Player* getInstance();
	~Player();

	const std::time_t& getTimeBetweenGameLaunchings();
	void setMapPosition(const cocos2d::Vec2& aPosition);
	const cocos2d::Vec2& getMapPosition();
};

#endif //PLAYER_H
