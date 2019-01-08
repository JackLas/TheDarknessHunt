#ifndef CITYSCENE_H
#define CITYSCENE_H 

#include "cocos2d.h"
#include "BaseScene.h"

class CityScene : public BaseScene
{
private:
	typedef BaseScene Parent;

	CityScene();
protected:
	CREATE_FUNC(CityScene);
	virtual bool init();

	void onButtonTouched(cocos2d::Ref* aSender, cocos2d::ui::Widget::TouchEventType aEvent);

public:
	static cocos2d::Scene* createScene();
	virtual ~CityScene();
};

#endif //CITYSCENE_H
