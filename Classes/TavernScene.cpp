#include "TavernScene.h"
#include "DataManager.h"
#include "MapScene.h"
#include "Player.h"

TavernScene::TavernScene()
	: mCurrentVisibleLayer(nullptr)
	, mHirePrice(0)
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

bool TavernScene::init()
{
	bool result = false;

	const std::string& view = DM->getViewById("TAVERN_SCENE");

	if (Parent::init(view))
	{
		result = true;
		setButtonTouchListener(CC_CALLBACK_2(TavernScene::onButtonTouched, this));
		cocos2d::Label* welcomeLabel = static_cast<cocos2d::Label*>(
			findChildByName({"welcome_layer", "welcome"})
		);
		if(welcomeLabel != nullptr)
		{
			const auto& welcomeQuotes = DM->getData().tavernData.welcomeSTIDs;
			int index = cocos2d::RandomHelper::random_int<int>(0, welcomeQuotes.size()-1);
			std::string welcomeStr = DM->getStringById(welcomeQuotes[index]) + " "
									+ DM->getStringById("STID_TAVERN_HELP");
			welcomeLabel->setString(welcomeStr);
		}

		replaceVisibleLayer("welcome_layer");
	}

	return result;
}

bool TavernScene::replaceVisibleLayer(const std::string& aLayerName)
{
	bool result = false;
	if (mCurrentVisibleLayer != nullptr)
	{
		mCurrentVisibleLayer->setVisible(false);
	}
	cocos2d::Node* layer = getChildByName(aLayerName);
	if (layer != nullptr)
	{
		mCurrentVisibleLayer = layer;
		mCurrentVisibleLayer->setVisible(true);
		result = true;
	}
	return result;
}

void TavernScene::initHireLayer()
{
	bool isHire = PLAYER->isNeedTeammate();
	cocos2d::Node* layer = getChildByName("hire_layer");
	if (layer != nullptr)
	{
		cocos2d::Label* description = layer->getChildByName<cocos2d::Label*>("description");
		if (description != nullptr)
		{
			if (isHire)
			{
				const auto& hireQuotes = DM->getData().tavernData.hireSTIDs;
				int index = cocos2d::RandomHelper::random_int<int>(0, hireQuotes.size() - 1);
				const std::string& descriptionStr = DM->getStringById(hireQuotes[index]);
				description->setString(descriptionStr);
			}
			else
			{
				const auto& noHireQuotes = DM->getData().tavernData.noHireSTIDs;
				int index = cocos2d::RandomHelper::random_int<int>(0, noHireQuotes.size() - 1);
				const std::string& descriptionStr = DM->getStringById(noHireQuotes[index]);
				description->setString(descriptionStr);
			}
		}

		cocos2d::Label* priceLabel = layer->getChildByName<cocos2d::Label*>("price");
		if (priceLabel != nullptr)
		{
			mHirePrice = DM->getData().tavernData.baseHirePrice;
			mHirePrice *= PLAYER->getCurrentHirePriceMultiplier();
			priceLabel->setString(std::to_string(mHirePrice));
			priceLabel->setVisible(isHire);
			
			const std::string iconName = "gold_icon";
			cocos2d::Sprite* goldIcon = layer->getChildByName<cocos2d::Sprite*>(iconName);
			if (isHire)
			{
				if (goldIcon == nullptr)
				{
					const auto& images = DM->getData().images;
					auto imageIt = images.find("ICON_COIN");
					if (imageIt != images.end())
					{
						goldIcon = cocos2d::Sprite::createWithSpriteFrameName(imageIt->second);
						if (goldIcon != nullptr)
						{
							goldIcon->setName(iconName);
							goldIcon->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_LEFT);
							layer->addChild(goldIcon);
						}
					}
				}
			}
			
			if (goldIcon != nullptr)
			{
				cocos2d::Vec2 iconPos;
				iconPos.x = priceLabel->getBoundingBox().getMaxX() + (goldIcon->getBoundingBox().size.width * 0.1f);
				iconPos.y = priceLabel->getPosition().y;
				goldIcon->setPosition(iconPos);
				goldIcon->setVisible(isHire);
			}
		}

		cocos2d::Node* btnHire = layer->getChildByName("btn_hire_positive");
		if (btnHire != nullptr)
		{
			btnHire->setVisible(isHire);
		}
	}
}

void TavernScene::initTellLayer()
{
	cocos2d::Node* layer = getChildByName("tell_layer");
	if (layer != nullptr)
	{
		cocos2d::Node* list = layer->getChildByName("list");
		if(list != nullptr)
		{
		}
	}
}

void TavernScene::showMessage(const std::string& aMsgSTID)
{
	const std::string& msg = DM->getStringById(aMsgSTID);
	cocos2d::Label* msgLabel = getChildByName<cocos2d::Label*>("message");
	if (msgLabel != nullptr)
	{
		msgLabel->setString(msg);
		const float& appearingTime = DM->getData().tavernData.messageAppearingTime;
		const float& showingTime = DM->getData().tavernData.messageShowingTime;
		msgLabel->runAction(cocos2d::Sequence::create(
			cocos2d::FadeIn::create(appearingTime),
			cocos2d::DelayTime::create(showingTime),
			cocos2d::FadeOut::create(appearingTime),
			nullptr
		));
	}
}

void TavernScene::onButtonTouched(cocos2d::Ref* aSender, cocos2d::ui::Widget::TouchEventType aEvent)
{
	if (aEvent == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		cocos2d::ui::Button* btn = static_cast<cocos2d::ui::Button*>(aSender);
		const std::string& btnName = btn->getName();
		if (btnName == "btn_map")
		{
			cocos2d::Director::getInstance()->replaceScene(MapScene::createScene());
		}
		else if (btnName == "btn_hire")
		{
			if (replaceVisibleLayer("hire_layer"))
			{
				initHireLayer();
			}
		}
		else if (btnName == "btn_tell")
		{
			if (replaceVisibleLayer("tell_layer"))
			{
				initTellLayer();
			}
		}
		else if (btnName == "btn_hire_positive")
		{
			replaceVisibleLayer("welcome_layer");
			if (PLAYER->spendGold(mHirePrice))
			{
				PLAYER->hireTeammate();
			}
			else
			{
				showMessage("STID_NO_GOLD");
			}
		}
		else if (btnName == "btn_back")
		{
			replaceVisibleLayer("welcome_layer");
		}
	}
}