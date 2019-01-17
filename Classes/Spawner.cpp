#include "Spawner.h"
#include "DataManager.h"

Spawner::Spawner(const std::vector<std::string>& aMonstersArray)
	: mMonsters(aMonstersArray)
{
}

Spawner::~Spawner()
{
}

Monster* Spawner::getNextMonster()
{
	Monster* result = nullptr;
	const auto monsters = DM->getData().monsters;
	
	if (mMonsters.size() > 0)
	{
		const int maxIndex = mMonsters.size() - 1;
		int monsterIndex = cocos2d::RandomHelper::random_int<int>(0, maxIndex);
		const auto monsterIt = monsters.find(mMonsters[monsterIndex]);
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