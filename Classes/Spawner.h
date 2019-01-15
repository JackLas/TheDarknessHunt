#ifndef SPAWNER_H
#define SPAWNER_H

#include "dataTypes.h"
#include "Monster.h"

class Spawner
{
private:
	std::vector<eMonsterID> const* mMonsters;
	cocos2d::Vec2 mSpawnPoint;

public:
	Spawner();
	virtual ~Spawner();
	Monster* getNextMonster();
	void setSpawnPoint(const cocos2d::Vec2& aPoint);
	void setMonsterArray(std::vector<eMonsterID> const* aArray);
};

#endif //SPAWNER_H