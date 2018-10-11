#ifndef POPUPLAYER_H
#define POPUPLAYER_H

#include "cocos2d.h"

class PopUpLayer : public cocos2d::Layer
{
private:
	typedef cocos2d::Layer Parent;

	float mAppearingTime;

protected:
	bool mIsActive;

	PopUpLayer();
	void deactivate();
	void activate();
	virtual bool init();

public:
	CREATE_FUNC(PopUpLayer);
	virtual ~PopUpLayer();

	virtual void show();
	virtual void hide();
	bool isActive();

	void setAppeatingTime(const float aAppearingTime);
};

#endif //POPUPLAYER_H
