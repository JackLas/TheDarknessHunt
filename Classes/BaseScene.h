#ifndef BASESCENE_H
#define BASESCENE_H

#include "cocos2d.h"
#include "ui/UIButton.h"

class BaseScene: public cocos2d::Layer
{
private:
	typedef cocos2d::Layer Parent;

	void initButtons(cocos2d::Vector<cocos2d::Node*>& aChildren, 
		const std::function<void (cocos2d::Ref* aSender, cocos2d::ui::Widget::TouchEventType aEvent)>& aCallback);
protected:
	BaseScene();
	virtual bool init(const std::string& aView);
	virtual void onEnter();
	void setButtonTouchListener(const std::function<void(cocos2d::Ref* aSender, cocos2d::ui::Widget::TouchEventType aEvent)>& aCallback);

	cocos2d::Node* findChildByName(const std::vector<std::string>& aSearchingSequence);

public:
	virtual ~BaseScene();
};

#endif //BASESCENE_H
