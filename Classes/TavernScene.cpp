#include "TavernScene.h"
#include "DataManager.h"
#include "MapScene.h"

TavernScene::TavernScene()
{
}

TavernScene::~TavernScene()
{
}

cocos2d::Scene* TavernScene::createScene()
{
	cocos2d::Scene* scene = cocos2d::Scene::create();
	cocos2d::Layer* layer = TavernScene::create();
	if (layer != nullptr)
	{
		scene->addChild(layer);
	}
	return scene;
}

void TavernScene::loadSpriteCache()
{
}

bool TavernScene::init()
{
	bool result = false;

	const std::string& view = DM->getViewById("TAVERN_SCENE");

	if (Parent::init(view))
	{
		result = true;
		setButtonTouchListener(CC_CALLBACK_2(TavernScene::onButtonTouched, this));
	}

	return result;
}

void TavernScene::onButtonTouched(cocos2d::Ref* aSender, cocos2d::ui::Widget::TouchEventType aEvent)
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