#ifndef TAVERNSCENE_H
#define TAVERNSCENE_H 

#include "cocos2d.h"
#include "BaseScene.h"

class TavernScene : public BaseScene
{
private:
	typedef BaseScene Parent;

	cocos2d::Node* mCurrentVisibleLayer;

	TavernScene();
protected:
	CREATE_FUNC(TavernScene);
	virtual bool init();

	bool replaceVisibleLayer(const std::string& aLayerName);
	void initHireLayer();
	void onButtonTouched(cocos2d::Ref* aSender, cocos2d::ui::Widget::TouchEventType aEvent);

public:
	static cocos2d::Scene* createScene();
	virtual ~TavernScene();
};

#endif //TAVERNSCENE_H
