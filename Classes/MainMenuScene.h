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

	virtual bool onTouchBegan(cocos2d::Touch* aTouch, cocos2d::Event* aEvent) override;

	void showEaster();
	short int mEasterCounter;
protected:
	virtual bool init() override;

public:
	static cocos2d::Scene* createScene();
};

#endif //MAINMENUSCENE_H
