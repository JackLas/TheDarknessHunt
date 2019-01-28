#ifndef MONSTER_H
#define MONSTER_H

#include "cocos2d.h"
#include "dataTypes.h"
class MonsterActionListener;

class Monster: public cocos2d::Sprite
{
private:
	typedef cocos2d::Sprite Parent;

	const sMonster& mData;
	float mCurrentHP;
	MonsterActionListener* mActionListener; 

	Monster(const sMonster& aMonsterData);
protected:
	virtual bool init();

public:
	static Monster* create(const sMonster& aMonsterData);
	virtual ~Monster();

	virtual void onEnter() override;
	void setActionListener(MonsterActionListener* aListener);
	
	const std::string& getName() const;
	float getCurrentHealth() const;
	float getCurrentHealthInPercent() const;
	const sResistance& getResistance() const;

	void onTouched();
};

#endif //MONSTER_H
