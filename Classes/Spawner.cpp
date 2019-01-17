#include "Spawner.h"
#include "DataManager.h"

Spawner::Spawner(const std::string& aLevelID)
	: mLevelID(aLevelID)
{
}

Spawner::~Spawner()
{
}

Monster* Spawner::getNextMonster()
{
	Monster* result = nullptr;

	const auto& data = DM->getData();
	const auto& allMonsters = data.monsters;
	auto levelIt = data.levels.find(mLevelID);
	if (levelIt != data.levels.end())
	{
		const auto& levelMonsters = levelIt->second.monsters;

		if (levelMonsters.size() > 0)
		{
			const int maxIndex = levelMonsters.size() - 1;
			int monsterIndex = cocos2d::RandomHelper::random_int<int>(0, maxIndex);
			const auto monsterIt = allMonsters.find(levelMonsters[monsterIndex]);
			if (monsterIt != allMonsters.end())
			{
				result = Monster::create(monsterIt->second);
				result->setPosition(mSpawnPoint);
			}
		}
	}

	return result;
}

void Spawner::setSpawnPoint(const cocos2d::Vec2& aPoint)
{
	mSpawnPoint = aPoint;
}