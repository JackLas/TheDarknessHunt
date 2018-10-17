#ifndef MAPSCENE_H
#define MAPSCENE_H

#include "cocos2d.h"

class MapScene: public cocos2d::Layer
{
private:
	typedef cocos2d::Layer Parent;

	MapScene();
protected:
	virtual bool init();
	virtual void onEnter();
	CREATE_FUNC(MapScene);

public:
	static cocos2d::Scene* createScene();
	virtual ~MapScene();
};

#endif //MAPSCENE_H
