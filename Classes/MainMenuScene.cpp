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

		initButtons();
	}

	return result;
}

void MainMenuScene::initButtons()
{
	std::vector<BMButton*> buttons;
	buttons.push_back(getChildByName<BMButton*>("btnPlay"));
	buttons.push_back(getChildByName<BMButton*>("btnOptions"));
	buttons.push_back(getChildByName<BMButton*>("btnExit"));

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
	if ((aEvent == cocos2d::ui::Widget::TouchEventType::ENDED) /*&& (!mOptionsPopUp->isActive())*/)
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
			/*if (mOptionsPopUp != nullptr)
			{
				mOptionsPopUp->show();
			}*/
		}
		else if (btnName == "btnExit")
		{
			cocos2d::Director::getInstance()->end();
		}
	}
}