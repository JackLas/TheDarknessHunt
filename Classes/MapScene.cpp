#include "MapScene.h"
#include "DataManager.h"
#include "MainMenuScene.h"
#include "CityScene.h"

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
	cache->addSpriteFramesWithFile("images/map_scene/map_scene.plist");
}

bool MapScene::init()
{
	const std::string& view = DM->getViewById("MAP_SCENE");
	bool result = false;

	if (Parent::init(view))
	{
		result = true;
		setButtonTouchListener(CC_CALLBACK_2(MapScene::onButtonTouched, this));
	}

	return result;
}

void MapScene::onButtonTouched(cocos2d::Ref* aSender, cocos2d::ui::Widget::TouchEventType aEvent)
{
	if (aEvent == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		cocos2d::ui::Button* btn = static_cast<cocos2d::ui::Button*>(aSender);
		const std::string& btnName = btn->getName();

		if (btnName == "back")
		{
			cocos2d::Director::getInstance()->replaceScene(MainMenuScene::createScene());
		}
		if (btnName == "city")
		{
			cocos2d::Director::getInstance()->replaceScene(CityScene::createScene());
		}
		else if (btnName == "tavern")
		{
		}
		else if (btnName == "camp")
		{
		}
	}
}