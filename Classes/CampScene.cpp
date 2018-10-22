#include "CampScene.h"
#include "DataManager.h"
#include "MapScene.h"

CampScene::CampScene()
{
}

CampScene::~CampScene()
{
}

cocos2d::Scene* CampScene::createScene()
{
	cocos2d::Scene* scene = cocos2d::Scene::create();
	cocos2d::Layer* layer = CampScene::create();
	if (layer != nullptr)
	{
		scene->addChild(layer);
	}
	return scene;
}

void CampScene::loadSpriteCache()
{
}

bool CampScene::init()
{
	bool result = false;

	const std::string& view = DM->getViewById("CAMP_SCENE");

	if (Parent::init(view))
	{
		result = true;
		setButtonTouchListener(CC_CALLBACK_2(CampScene::onButtonTouched, this));
	}

	return result;
}

void CampScene::onButtonTouched(cocos2d::Ref* aSender, cocos2d::ui::Widget::TouchEventType aEvent)
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