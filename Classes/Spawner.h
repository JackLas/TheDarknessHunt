#ifndef SPAWNER_H
#define SPAWNER_H

#include "dataTypes.h"
#include "Monster.h"

class Spawner
{
private:
	const std::vector<eMonsterID>& mMonsters;
	cocos2d::Vec2 mSpawnPoint;

public:
	Spawner(const std::vector<eMonsterID>& aMonstersArray);
	virtual ~Spawner();
	Monster* getNextMonster();
	void setSpawnPoint(const cocos2d::Vec2& aPoint);
};

#endif //SPAWNER_H