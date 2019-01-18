#include "MainMenuScene.h"
#include "DataManager.h"
#include "MapScene.h"
#include "PopUpLayer.h"
#include "Player.h"

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
	
	if (Parent::init(DM->getViewById("MAIN_MENU")))
	{
		result = true;
		setButtonTouchListener(CC_CALLBACK_2(MainMenuScene::onButtonTouched, this));

		PopUpLayer* optionsLayer = getChildByName<PopUpLayer*>("options_layer");
		if (optionsLayer != nullptr)
		{
			cocos2d::Node* flagSelected = optionsLayer->getChildByName("flag_selected");
			if (flagSelected != nullptr)
			{
				cocos2d::Node* flagLocaleButton = optionsLayer->getChildByName(DM->getSettings().locale);
				if (flagLocaleButton != nullptr)
				{
					flagSelected->setPosition(flagLocaleButton->getPosition());
				}
			}
		}

		cocos2d::Sprite* logo = getChildByName<cocos2d::Sprite*>("logo");
		if (logo != nullptr)
		{

			cocos2d::MoveBy* action = cocos2d::MoveBy::create(0.8f, cocos2d::Vec2(0.0f, getContentSize().height * -0.01f));
			logo->runAction(cocos2d::RepeatForever::create(cocos2d::Sequence::create(action, action->reverse(), nullptr)));
		}

		// --- Testing elapsed time between launchings game ---
		time_t elapsedTime = Player::getInstance()->getTimeBetweenGameLaunchings();
		std::string timeStr;
		timeStr += std::to_string(elapsedTime / 86400);
		elapsedTime = elapsedTime % 86400;
		timeStr += "::" + std::to_string(elapsedTime / 3600);
		elapsedTime = elapsedTime % 3600;
		timeStr += ":" + std::to_string(elapsedTime / 60);
		elapsedTime = elapsedTime % 60;
		timeStr += ":" + std::to_string(elapsedTime) + "\n\n\n";

		auto fontIt = DM->getData().fonts.find("SMALL_FONT");
		if (fontIt != DM->getData().fonts.end())
		{ 
			cocos2d::Label* timeLabel = cocos2d::Label::createWithBMFont(fontIt->second, timeStr);
			timeLabel->setAnchorPoint(cocos2d::Vec2::ANCHOR_BOTTOM_LEFT);
			addChild(timeLabel);
		}

		// --- end of test ---
	}

	return result;
}

void MainMenuScene::onButtonTouched(cocos2d::Ref* aSender, cocos2d::ui::Widget::TouchEventType aEvent)
{
	if (aEvent == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		PopUpLayer* optionsLayer = getChildByName<PopUpLayer*>("options_layer");
		bool isMainMenu = true;
		if (optionsLayer != nullptr)
		{
			isMainMenu = !optionsLayer->isActive();
		}

		cocos2d::Node* btn = static_cast<cocos2d::Node*>(aSender);
		const std::string& btnName = btn->getName();

		if (isMainMenu)
		{
			if (btnName == "btnPlay")
			{
				//cocos2d::Director::getInstance()->pushScene(MapScene::createScene());
				cocos2d::Director::getInstance()->replaceScene(MapScene::createScene());
			}
			else if (btnName == "btnOptions")
			{
				optionsLayer->show();
			}
			else if (btnName == "btnExit")
			{
				cocos2d::Director::getInstance()->end();
			}
		}
		else
		{
			if (btnName == "btnOK")
			{
				optionsLayer->hide();
				DM->saveSettings();
			}
			else if (btnName == "en" || btnName == "ru")
			{
				cocos2d::Node* flagSelected = optionsLayer->getChildByName("flag_selected");
				flagSelected->setPosition(btn->getPosition());
				std::string& locale = DM->getSettings().locale;
				if (locale != btnName)
				{
					locale = btnName;
					//reload strings
				}
			}
		}
	}
}