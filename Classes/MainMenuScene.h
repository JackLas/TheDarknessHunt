#ifndef MAINMENUSCENE_H
#define MAINMENUSCENE_H

#include "cocos2d.h"

class MainMenuScene : public cocos2d::Layer
{
private:
	typedef cocos2d::Layer Parent;

	MainMenuScene();
	CREATE_FUNC(MainMenuScene);
protected:
	virtual bool init();
public:
	static cocos2d::Scene* createScene();
};
#endif //MAINMENUSCENE_H
