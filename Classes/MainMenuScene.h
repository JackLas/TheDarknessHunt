#ifndef MAINMENUSCENE_H
#define MAINMENUSCENE_H

#include "cocos2d.h"
#include "BaseScene.h"

#include "ui/UIButton.h"

class MainMenuScene: public BaseScene
{
private:
	typedef BaseScene Parent;

	MainMenuScene();
	CREATE_FUNC(MainMenuScene);
	void onButtonTouched(cocos2d::Ref* aSender, cocos2d::ui::Widget::TouchEventType aEvent);

protected:
	virtual bool init();
	virtual void loadSpriteCache();
public:
	static cocos2d::Scene* createScene();
};

#endif //MAINMENUSCENE_H
