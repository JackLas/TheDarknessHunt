#ifndef MAINMENUSCENE_H
#define MAINMENUSCENE_H

#include "cocos2d.h"
#include "ui/UIButton.h"

class MainMenuScene : public cocos2d::Layer
{
private:
	typedef cocos2d::Layer Parent;

	MainMenuScene();
	CREATE_FUNC(MainMenuScene);
	void onButtonTouched(cocos2d::Ref* aSender, cocos2d::ui::Widget::TouchEventType aEvent);
	void initButtons();
protected:
	virtual bool init();
public:
	static cocos2d::Scene* createScene();
};
#endif //MAINMENUSCENE_H
