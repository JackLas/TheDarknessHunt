#include "CityScene.h"
#include "DataManager.h"
#include "MapScene.h"

CityScene::CityScene()
{
}

CityScene::~CityScene()
{
}

cocos2d::Scene* CityScene::createScene()
{
	cocos2d::Scene* scene = cocos2d::Scene::create();
	cocos2d::Layer* layer = CityScene::create();
	if (layer != nullptr)
	{
		scene->addChild(layer);
	}
	return scene;
}

void CityScene::loadSpriteCache()
{
}

bool CityScene::init()
{
	bool result = false;

	const std::string& view = DM->getViewById("CITY_SCENE");

	if (Parent::init(view))
	{
		result = true;
		setButtonTouchListener(CC_CALLBACK_2(CityScene::onButtonTouched, this));
	}

	return result;
}

void CityScene::onButtonTouched(cocos2d::Ref* aSender, cocos2d::ui::Widget::TouchEventType aEvent)
{
	if (aEvent == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		cocos2d::ui::Button* btn = static_cast<cocos2d::ui::Button*>(aSender);
		const std::string& btnName = btn->getName();
		if (btnName == "back")
		{
			cocos2d::Director::getInstance()->replaceScene(MapScene::createScene());
		}
	}
}