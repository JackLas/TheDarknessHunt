#ifndef CAMPSCENE_H
#define CAMPSCENE_H 

#include "cocos2d.h"
#include "BaseScene.h"

class CampScene : public BaseScene
{
private:
	typedef BaseScene Parent;

	CampScene();
protected:
	CREATE_FUNC(CampScene);
	virtual bool init();

	void onButtonTouched(cocos2d::Ref* aSender, cocos2d::ui::Widget::TouchEventType aEvent);

public:
	static cocos2d::Scene* createScene();
	virtual ~CampScene();
};

#endif //CAMPSCENE_H
