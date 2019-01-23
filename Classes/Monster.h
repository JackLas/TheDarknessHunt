#ifndef MONSTER_H
#define MONSTER_H

#include "cocos2d.h"
#include "dataTypes.h"
#include "MonsterDeathListener.h"

class Monster: public cocos2d::Sprite
{
private:
	typedef cocos2d::Sprite Parent;

	const sMonster& mData;
	float mCurrentHP;
	MonsterDeathListener* mDeathListener;

	Monster(const sMonster& aMonsterData);
protected:
	virtual bool init();

public:
	static Monster* create(const sMonster& aMonsterData);
	virtual ~Monster();

	const std::string& getName() const;
	float getCurrentHealth() const;
	float getCurrentHealthInPercent() const;
	void onTouched();
	void setDeathListener(MonsterDeathListener* aListener);
};

#endif //MONSTER_H
