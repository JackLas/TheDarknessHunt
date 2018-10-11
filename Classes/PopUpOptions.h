#ifndef POPUPOPTIONS_H
#define POPUPOPTIONS_H

#include "PopUpLayer.h"
#include "ui/UIButton.h"

class PopUpOptions : public PopUpLayer
{
private:
	typedef PopUpLayer Parent;

	void onButtonTouched(cocos2d::Ref* aSender, cocos2d::ui::Widget::TouchEventType aEvent);
	void initButtons();
protected:
	PopUpOptions();
	virtual bool init();
public:
	CREATE_FUNC(PopUpOptions);
	virtual ~PopUpOptions();
};

#endif //POPUPOPTIONS_H
