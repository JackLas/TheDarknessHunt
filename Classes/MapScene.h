#ifndef MAPSCENE_H
#define MAPSCENE_H

#include "cocos2d.h"
#include "BaseScene.h"
#include "ui/UIButton.h"

class MapScene: public BaseScene
{
private:
	typedef BaseScene Parent;

	MapScene();
protected:
	CREATE_FUNC(MapScene);

	virtual bool init();

	void onButtonTouched(cocos2d::Ref* aSender, cocos2d::ui::Widget::TouchEventType aEvent);

public:
	static cocos2d::Scene* createScene();
	virtual ~MapScene();
};

#endif //MAPSCENE_H
