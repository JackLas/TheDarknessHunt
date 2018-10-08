#ifndef MAINMENUSCENE_H
#define MAINMENUSCENE_H

#include "cocos2d.h"

class MainMenuScene : public cocos2d::Layer
{
private:
	typedef cocos2d::Layer Parent;


	MainMenuScene();
protected:
	virtual bool init();
public:
	CREATE_FUNC(MainMenuScene);
};
#endif //MAINMENUSCENE_H
