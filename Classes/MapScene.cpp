#include "MapScene.h"
#include "DataManager.h"

MapScene::MapScene()
{
}

MapScene::~MapScene()
{
}

cocos2d::Scene* MapScene::createScene()
{
	cocos2d::Scene* scene = cocos2d::Scene::create();
	cocos2d::Layer* layer = MapScene::create();
	if (layer != nullptr)
	{
		scene->addChild(layer);
	}
	return scene;
}

void MapScene::loadSpriteCache()
{
	cocos2d::SpriteFrameCache* cache = cocos2d::SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("images/images.plist");
}

bool MapScene::init()
{
	const std::string& view = DM->getViewById("MAP_SCENE");
	return Parent::init(view);
}