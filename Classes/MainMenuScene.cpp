#include "MainMenuScene.h"
#include "DataManager.h"
#include "MapScene.h"

#include "PopUpLayer.h"

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

void MainMenuScene::loadSpriteCache()
{
	cocos2d::SpriteFrameCache* cache = cocos2d::SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("images/images.plist");
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
	}

	return result;
}

void MainMenuScene::onButtonTouched(cocos2d::Ref* aSender, cocos2d::ui::Widget::TouchEventType aEvent)
{
	if (aEvent == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		PopUpLayer* optionsLayer = getChildByName<PopUpLayer*>("options_layer");

		if (optionsLayer != nullptr)
		{
			cocos2d::Node* btn = static_cast<cocos2d::Node*>(aSender);
			const std::string& btnName = btn->getName();

			if (!optionsLayer->isActive())
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
}