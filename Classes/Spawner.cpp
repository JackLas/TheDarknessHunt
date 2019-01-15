#include "Spawner.h"
#include "DataManager.h"

Spawner::Spawner()
	: mMonsters(nullptr)
{
}

Spawner::~Spawner()
{
}

Monster* Spawner::getNextMonster()
{
	Monster* result = nullptr;
	const auto monsters = DM->getData().monsters;
	
	if (mMonsters != nullptr)
	{
		int monsterIndex = cocos2d::RandomHelper::random_int<int>(0, mMonsters->size()-1);
		const auto monsterIt = monsters.find(eMonsterID::MONSTER_SKELETON);
		if (monsterIt != monsters.end())
		{
			result = Monster::create(monsterIt->second);
			result->setPosition(mSpawnPoint);
		}
	}

	return result;
}

void Spawner::setSpawnPoint(const cocos2d::Vec2& aPoint)
{
	mSpawnPoint = aPoint;
}

void Spawner::setMonsterArray(std::vector<eMonsterID> const* aArray)
{
	mMonsters = aArray;
}