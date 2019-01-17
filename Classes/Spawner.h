#ifndef SPAWNER_H
#define SPAWNER_H

#include "dataTypes.h"
#include "Monster.h"

class Spawner
{
private:
	const std::string& mLevelID;
	cocos2d::Vec2 mSpawnPoint;

public:
	Spawner(const std::string& aLevelID);
	virtual ~Spawner();
	Monster* getNextMonster();
	void setSpawnPoint(const cocos2d::Vec2& aPoint);
};

#endif //SPAWNER_H