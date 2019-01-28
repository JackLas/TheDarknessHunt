#ifndef MONSTERACTIONLISTENER_H
#define MONSTERACTIONLISTENER_H

class Monster;

class MonsterActionListener
{
public:
	virtual void onMonsterDied() = 0;
	virtual void onMonsterSpawned(const Monster* aMonster) = 0;
};

#endif //MONSTERACTIONLISTENER_H