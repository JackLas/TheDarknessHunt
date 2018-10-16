#include "MainMenuScene.h"
#include "DataManager.h"
#include "ViewBuilder.h"

MainMenuScene::MainMenuScene()
	: mOptions(nullptr)
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

		cocos2d::SpriteFrameCache* cache = cocos2d::SpriteFrameCache::getInstance();
		cache->addSpriteFramesWithFile("images/images.plist");

		const std::string& view = DM->getViewById("MAIN_MENU");
		result &= ViewBuilder::loadFromJson(this, view);
		
		mOptions = PopUpOptions::create();
		if (mOptions != nullptr)
		{
			addChild(mOptions);
		}

		initButtons();
	}

	return result;
}

void MainMenuScene::initButtons()
{
	std::vector<cocos2d::ui::Button*> buttons;
	buttons.push_back(getChildByName<cocos2d::ui::Button*>("btnPlay"));
	buttons.push_back(getChildByName<cocos2d::ui::Button*>("btnOptions"));
	buttons.push_back(getChildByName<cocos2d::ui::Button*>("btnExit"));

	for (auto& button : buttons)
	{
		if (button != nullptr)
		{
			button->addTouchEventListener(CC_CALLBACK_2(MainMenuScene::onButtonTouched, this));
		}
	}
}

void MainMenuScene::onButtonTouched(cocos2d::Ref* aSender, cocos2d::ui::Widget::TouchEventType aEvent)
{
	if ((aEvent == cocos2d::ui::Widget::TouchEventType::ENDED) &&(!mOptions->isActive()))
	{
		cocos2d::Node* btn = static_cast<cocos2d::Node*>(aSender);
		const std::string& btnName = btn->getName();
		if (btnName == "btnPlay")
		{
			//cocos2d::Director::getInstance()->pushScene(MapScene::createScene());
			//test
			//if (mLogo->getNumberOfRunningActions() == 0)
			//{
			//	mLogo->runAction(cocos2d::RotateBy::create(0.5f, 360));
			//}

		}
		else if (btnName == "btnOptions")
		{
			if (mOptions != nullptr)
			{
				mOptions->show();
			}
		}
		else if (btnName == "btnExit")
		{
			cocos2d::Director::getInstance()->end();
		}
	}
}