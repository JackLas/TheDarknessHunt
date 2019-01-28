#ifndef MONSTERACTIONLISTENER_H
#define MONSTERACTIONLISTENER_H

class Monster;

class MonsterActionListener
{
public:
	virtual void onMonsterDied(Monster* aMonster) = 0;
	virtual void onMonsterSpawned(const Monster* aMonster) = 0;
	virtual void onHealthRestored(const Monster* aMonster) = 0;
	virtual void onHealingTimerUpdated(const Monster* aMonster, const float& aTimeLeft) = 0;
};

#endif //MONSTERACTIONLISTENER_H