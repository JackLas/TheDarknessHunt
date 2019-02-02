#include "MainMenuScene.h"
#include "DataManager.h"
#include "MapScene.h"
#include "PopUpLayer.h"
#include "Player.h"

MainMenuScene::MainMenuScene()
	: mEasterCounter(0)
	, mIsSettingsChanged(false)
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

		auto touchListener = cocos2d::EventListenerTouchOneByOne::create();
		touchListener->setSwallowTouches(true);
		touchListener->onTouchBegan = CC_CALLBACK_2(MainMenuScene::onTouchBegan, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
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
				if (mEasterCounter == 5)
				{
					showEaster();
				}
				else
				{
					cocos2d::Director::getInstance()->replaceScene(MapScene::createScene());
				}
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
				
				if (mIsSettingsChanged)
				{
					DM->saveSettings();
					DM->reloadStrings();
					cocos2d::Director::getInstance()->replaceScene(MainMenuScene::createScene());
					mIsSettingsChanged = false;
				}
			}
			else if (btnName == "btn_reset")
			{
				PLAYER->resetProgress();
			}
			else if (btnName == "en" || btnName == "ru")
			{
				cocos2d::Node* flagSelected = optionsLayer->getChildByName("flag_selected");
				flagSelected->setPosition(btn->getPosition());
				std::string& locale = DM->getSettings().locale;
				if (locale != btnName)
				{
					locale = btnName;
					mIsSettingsChanged = true;
				}
			}
		}
	}
}

bool MainMenuScene::onTouchBegan(cocos2d::Touch* aTouch, cocos2d::Event* aEvent)
{
	cocos2d::Sprite* logo = getChildByName<cocos2d::Sprite*>("logo");
	if (logo != nullptr)
	{
		const cocos2d::Vec2 touchPosition = convertTouchToNodeSpace(aTouch);
		if (logo->getBoundingBox().containsPoint(touchPosition))
		{
			if (mEasterCounter < 5)
			{
				mEasterCounter++;
			}
			else
			{
				mEasterCounter = 0;
			}
		}
	}
	return false;
}

void MainMenuScene::showEaster()
{
	const auto& images = DM->getData().images;
	auto frameNameIt = images.find("MR_PIN");
	if (frameNameIt != images.end())
	{
		const std::string& frameName = frameNameIt->second;
		cocos2d::Sprite* mrPin = cocos2d::Sprite::createWithSpriteFrameName(frameName);
		if (mrPin != nullptr)
		{
			const cocos2d::Size& sceneSize = getBoundingBox().size;
			const cocos2d::Size& mrPinSize = mrPin->getBoundingBox().size;

			mrPin->setPosition(sceneSize.width + (mrPinSize.width / 2.0f), sceneSize.height / 3.0f);
			cocos2d::Vec2 endPosition(mrPinSize.width / -2.0f, sceneSize.height / 2.0f);
			float jumpHeight = sceneSize.height / 4.0f;
			float animTime = 1.2f;
			mrPin->runAction(cocos2d::Sequence::create(
				cocos2d::Spawn::create(
					cocos2d::JumpTo::create(animTime, endPosition, jumpHeight, 1),
					cocos2d::RotateBy::create(animTime, -360.0f),
					nullptr
				),
				cocos2d::CallFunc::create([mrPin]() { mrPin->removeFromParent(); }),
				nullptr
			));
			addChild(mrPin);
		}
	}
	
}
