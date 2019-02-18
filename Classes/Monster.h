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
	int mCurrentHP;
	float mHealingTimer;
	float mElapsedHealingTime;

	MonsterActionListener* mActionListener; 

	Monster(const sMonster& aMonsterData);
protected:
	virtual bool init() override;

public:
	static Monster* create(const sMonster& aMonsterData);
	virtual ~Monster();

	virtual void onEnter() override;
	void setActionListener(MonsterActionListener* aListener);
	
	virtual void update(float aDeltaTime) override;

	const sMonster& getData() const;
	const std::string& getName() const override;
	int getCurrentHealth() const;
	float getCurrentHealthInPercent() const;
	const sResistance& getResistance() const;
	unsigned int getGoldReward() const;

	void takeDamage(const sDamage& aDealtDamage);
	void startDeathAnimation(cocos2d::FiniteTimeAction* aAnimation, bool aDirection);
};

#endif //MONSTER_H
