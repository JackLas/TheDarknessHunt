#include "MainMenuScene.h"
#include "DataManager.h"
#include "ViewBuilder.h"

MainMenuScene::MainMenuScene()
{
}

cocos2d::Scene* MainMenuScene::createScene()
{
	cocos2d::Scene* scene = cocos2d::Scene::create();
	cocos2d::Layer* layer = MainMenuScene::create();
	if (layer != nullptr)
	{
		scene->addChild(layer);
	}
	return scene;
}

bool MainMenuScene::init()
{
	bool result = false;
	
	if (Parent::init())
	{
		result = true;
		const sData& data = DM->getData();
		cocos2d::SpriteFrameCache* cache = cocos2d::SpriteFrameCache::getInstance();
		cache->addSpriteFramesWithFile("images/images.plist");

		const auto viewIt = data.views.find("MAIN_MENU");
		if (viewIt != data.views.end())
		{
			result &= ViewBuilder::loadFromJson(this, viewIt->second);
		}	
	}

	return result;
}