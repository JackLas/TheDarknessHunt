#include "MapScene.h"
#include "ViewBuilder.h"
#include "DataManager.h"

MapScene::MapScene()
{
}

MapScene::~MapScene()
{
}

cocos2d::Scene* MapScene::createScene()
{
	cocos2d::Layer* layer = MapScene::create();
	cocos2d::Scene* scene = cocos2d::Scene::create();
	if (layer != nullptr)
	{
		scene->addChild(layer);
	}
	return scene;
}

bool MapScene::init()
{
	bool result = false;

	cocos2d::SpriteFrameCache* cache = cocos2d::SpriteFrameCache::getInstance();
	cache->removeUnusedSpriteFrames();
	cocos2d::Director::getInstance()->getTextureCache()->removeUnusedTextures();

	cache->addSpriteFramesWithFile("images/images.plist");

	if (Parent::init())
	{
		result = true;

		const std::string& view = DM->getViewById("MAP_SCENE");
		result &= ViewBuilder::loadFromJson(this, view);
	}

	return result;
}