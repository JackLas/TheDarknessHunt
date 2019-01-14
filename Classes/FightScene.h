#ifndef FIGHTSCENE_H
#define FIGHTSCENE_H

#include "BaseScene.h"
#include "cocos2d.h"
#include "dataTypes.h"

class FightScene : public BaseScene
{
private:
	typedef BaseScene Parent;

	const sLevel& mLevelData;

	FightScene(const sLevel& aLevelData);
protected:
	static FightScene* create(const sLevel& aLevelData);
	virtual bool init();
	void onButtonTouched(cocos2d::Ref* aSender, cocos2d::ui::Widget::TouchEventType aEvent);
	virtual void onEnter();
public:
	static cocos2d::Scene* createScene(const sLevel& aLevelData);
	virtual ~FightScene();
};

#endif //FIGHTSCENE_H