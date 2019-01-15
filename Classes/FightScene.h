#ifndef FIGHTSCENE_H
#define FIGHTSCENE_H

#include "BaseScene.h"
#include "cocos2d.h"
#include "dataTypes.h"
#include "Monster.h"
#include "Spawner.h"
#include "MonsterDeathListener.h"

class FightScene : public BaseScene, public MonsterDeathListener
{
private:
	typedef BaseScene Parent;

	const sLevel mLevelData;
	Spawner mSpawner;
	Monster* mCurrentMonster;

	FightScene(const sLevel& aLevelData);
protected:
	static FightScene* create(const sLevel& aLevelData);
	virtual bool init();
	void onButtonTouched(cocos2d::Ref* aSender, cocos2d::ui::Widget::TouchEventType aEvent);
	virtual void onEnter();

	virtual bool onTouchBegan(cocos2d::Touch* aTouch, cocos2d::Event* aEvent);
	virtual void onTouchMoved(cocos2d::Touch* aTouch, cocos2d::Event* aEvent);
	virtual void onTouchEnded(cocos2d::Touch* aTouch, cocos2d::Event* aEvent);

	void updateMonster();
public:
	static cocos2d::Scene* createScene(const sLevel& aLevelData);
	virtual ~FightScene();

	virtual void onMonsterDied();
};

#endif //FIGHTSCENE_H