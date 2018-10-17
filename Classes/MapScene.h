#ifndef MAPSCENE_H
#define MAPSCENE_H

#include "BaseScene.h"

class MapScene: public BaseScene
{
private:
	typedef BaseScene Parent;

	MapScene();
protected:
	CREATE_FUNC(MapScene);

	virtual bool init();
	virtual void loadSpriteCache();

public:
	static cocos2d::Scene* createScene();
	virtual ~MapScene();
};

#endif //MAPSCENE_H
