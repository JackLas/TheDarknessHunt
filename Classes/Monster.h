#ifndef MONSTER_H
#define MONSTER_H

#include "cocos2d.h"
#include "dataTypes.h"

class Monster: public cocos2d::Sprite
{
private:
	typedef cocos2d::Sprite Parent;

	const sMonster& mData;
	float mCurrentHealth;

	Monster(const sMonster& aMonsterData);
protected:
	virtual bool init();

public:
	static Monster* create(const sMonster& aMonsterData);
	virtual ~Monster();

	const std::string& getName() const;
	float getCurrentHealth() const;
	void onTouched();
};

#endif //MONSTER_H
