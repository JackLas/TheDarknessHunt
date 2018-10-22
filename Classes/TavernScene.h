#ifndef TAVERNSCENE_H
#define TAVERNSCENE_H 

#include "cocos2d.h"
#include "BaseScene.h"

class TavernScene : public BaseScene
{
private:
	typedef BaseScene Parent;

	TavernScene();
protected:
	CREATE_FUNC(TavernScene);
	virtual bool init();
	virtual void loadSpriteCache();

	void onButtonTouched(cocos2d::Ref* aSender, cocos2d::ui::Widget::TouchEventType aEvent);

public:
	static cocos2d::Scene* createScene();
	virtual ~TavernScene();
};

#endif //TAVERNSCENE_H
