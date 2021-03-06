#ifndef FIGHTSCENE_H
#define FIGHTSCENE_H

#include "BaseScene.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "dataTypes.h"
#include "Monster.h"
#include "Spawner.h"
#include "MonsterActionListener.h"

class FightScene : public BaseScene, public MonsterActionListener
{
private:
	typedef BaseScene Parent;

	const std::string mLevelID;
	Spawner mSpawner;
	Monster* mCurrentMonster;

	cocos2d::Label* mPhysResistLabel;
	cocos2d::Label* mMagResistLabel;
	cocos2d::Label* mKillsLabel;
	cocos2d::Label* mGoldLabel;
	cocos2d::Vec2 mGoldIconPosition;
	cocos2d::Label* mMonsterNameLabel;
	cocos2d::Label* mTimeToHealLabel;
	cocos2d::ui::LoadingBar* mMonsterHealthBar;

	bool mIsMonsterDeathAnimationDirectionRight;

	FightScene(const std::string& aLevelID);
protected:
	static FightScene* create(const std::string& aLevelID);
	virtual bool init() override;
	void onButtonTouched(cocos2d::Ref* aSender, cocos2d::ui::Widget::TouchEventType aEvent);
	virtual void onEnter() override;

	virtual bool onTouchBegan(cocos2d::Touch* aTouch, cocos2d::Event* aEvent) override;
	virtual void onTouchMoved(cocos2d::Touch* aTouch, cocos2d::Event* aEvent) override;
	virtual void onTouchEnded(cocos2d::Touch* aTouch, cocos2d::Event* aEvent) override;

	void givePlayerReward(const Monster* aMonster);
	void updateGoldLabel();
	void updateKillsLabel();
	void respawnMonster();
public:
	static cocos2d::Scene* createScene(const std::string& aLevelID);
	virtual ~FightScene();

	virtual void onMonsterDied(Monster* aMonster) override;
	virtual void onMonsterSpawned(const Monster* aMonster) override;
	virtual void onHealthRestored(const Monster* aMonster) override;
	virtual void onHealingTimerUpdated(const Monster* aMonster, const float& aTimeLeft) override;
	virtual void onDamageDealt(const Monster* aMonster) override;

};

#endif //FIGHTSCENE_H