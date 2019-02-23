#include "CityScene.h"
#include "DataManager.h"
#include "MapScene.h"
#include "Player.h"

CityScene::CityScene()
	: mGoldLabel(nullptr)
	, mItemNameLabel(nullptr)
	, mPhysDmgLabel(nullptr)
	, mMagDmgLabel(nullptr)
	, mPrice(nullptr)
	, mItemPreview(nullptr)
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

bool CityScene::init()
{
	bool result = false;

	const std::string& view = DM->getViewById("CITY_SCENE");
	if (Parent::init(view))
	{
		result = true;
		setButtonTouchListener(CC_CALLBACK_2(CityScene::onButtonTouched, this));
		
		mGoldLabel = static_cast<cocos2d::Label*>(findChildByName({"gold_bar", "text"}));
		mItemNameLabel = getChildByName<cocos2d::Label*>("item_name");
		mPhysDmgLabel = getChildByName<cocos2d::Label*>("phys_dmg_label");
		mMagDmgLabel = getChildByName<cocos2d::Label*>("mag_dmg_label");
		mPrice = getChildByName<cocos2d::Label*>("price");
		mItemPreview = static_cast<cocos2d::Sprite*>(findChildByName({"item_frame", "content"}));

		updatePlayerGold();
		initItemList();
	}

	return result;
}

void CityScene::updatePlayerGold()
{
	if (mGoldLabel != nullptr)
	{
		unsigned int gold = PLAYER->getGold();
		mGoldLabel->setString(std::to_string(gold));
	}
}

void CityScene::initItemList()
{
	cocos2d::ui::ScrollView* list = getChildByName<cocos2d::ui::ScrollView*>("item_list");
	if (list != nullptr)
	{
		const auto& items = DM->getData().items;
		const auto& images = DM->getData().images;
		const std::string& templatePath = DM->getViewById("TEMPLATE_INVENTORY_ROW");
		cocos2d::Node* row = DM->createViewFromJson(templatePath);
		if (row != nullptr)
		{
			const int itemsInRow = row->getChildrenCount();
			int numberOfRows = static_cast<int>(std::ceil(items.size() / static_cast<float>(itemsInRow)));
			cocos2d::Size scrollViewInnerSize = list->getInnerContainerSize();
			const int rowHeight = row->getBoundingBox().size.height;
			scrollViewInnerSize.height = numberOfRows * rowHeight;
			list->setInnerContainerSize(scrollViewInnerSize);
			cocos2d::Vec2 rowPosition = list->getInnerContainerSize();
			rowPosition.x /= 2.0f;

			for (auto itemIt = items.begin(); itemIt != items.end();)
			{
				for (auto child : row->getChildren())
				{
					if (itemIt == items.end())
					{
						break;
					}

					const sItem& item = itemIt->second;
					child->setName(itemIt->first);

					const std::string& frameNameID = item.frameNameID;
					const auto frameName = images.find(frameNameID);
					if (frameName != images.end())
					{
						cocos2d::Sprite* itemSprite = cocos2d::Sprite::createWithSpriteFrameName(frameName->second);
						if (itemSprite != nullptr)
						{
							cocos2d::Vec2 pos = child->getBoundingBox().size / 2.0f;
							itemSprite->setPosition(pos);
							child->addChild(itemSprite);
						}
					}

					cocos2d::ui::Button* btn = static_cast<cocos2d::ui::Button*>(child);
					btn->addTouchEventListener(CC_CALLBACK_2(CityScene::onShopSlotTouched, this));

					++itemIt;
				}
				row->setPosition(rowPosition);
				list->addChild(row);
				row = DM->createViewFromJson(templatePath);
				rowPosition.y -= rowHeight;
			}
		}
	}
}

void CityScene::onShopSlotTouched(cocos2d::Ref* aSender, cocos2d::ui::Widget::TouchEventType aEvent)
{
	if (aEvent == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		cocos2d::Node* sender = static_cast<cocos2d::Node*>(aSender);
		mSelectedItemID = sender->getName();
		setSelectedFrame(sender);
		setSelectedItemInfo(mSelectedItemID);
	}
}

void CityScene::setSelectedFrame(cocos2d::Node* aItemBtn)
{
	const std::string selectedFrameName = "frame";
	cocos2d::ui::ScrollView* list = getChildByName<cocos2d::ui::ScrollView*>("item_list");
	if (list != nullptr)
	{
		cocos2d::Sprite* frame = static_cast<cocos2d::Sprite*>(
			list->getChildByName(selectedFrameName));
		if (frame == nullptr)
		{
			const auto images = DM->getData().images;
			auto imageIt = images.find("SELECTED_ITEM_FRAME");
			if (imageIt != images.end())
			{
				frame = cocos2d::Sprite::createWithSpriteFrameName(imageIt->second);
				if (frame != nullptr)
				{
					frame->setName(selectedFrameName);
					list->addChild(frame);
				}
			}
		}
		if (frame != nullptr)
		{
			cocos2d::Node* senderParent = aItemBtn->getParent();
			if (senderParent != nullptr)
			{
				cocos2d::Vec2 framePosition;
				framePosition.x = senderParent->getBoundingBox().getMinX();
				framePosition.y = senderParent->getBoundingBox().getMinY();
				framePosition += aItemBtn->getPosition();
				frame->setPosition(framePosition);
			}
		}
	}
}

void CityScene::setSelectedItemInfo(const std::string& aSelectedItemID)
{
	const auto& items = DM->getData().items;
	const auto itemIt = items.find(aSelectedItemID);
	if (itemIt != items.end())
	{
		const sItem& item = itemIt->second;
		if (mItemNameLabel &&
			mPhysDmgLabel &&
			mMagDmgLabel &&
			mPrice &&
			mItemPreview)
		{
			mItemNameLabel->setString(DM->getStringById(item.name));
			mPhysDmgLabel->setString(std::to_string(item.damage.physical));
			mMagDmgLabel->setString(std::to_string(item.damage.magical));

			const auto& images = DM->getData().images;
			auto frameNameIt = images.find(item.frameNameID);
			if (frameNameIt != images.end())
			{
				mItemPreview->initWithSpriteFrameName(frameNameIt->second);
			}

			std::string priceStr = std::to_string(item.price);
			mPrice->setString(priceStr);
			if (PLAYER->isGoldEnough(item.price))
			{
				mPrice->setColor(cocos2d::Color3B::WHITE);
			}
			else
			{
				mPrice->setColor(cocos2d::Color3B::RED);
			}
		}
	}
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
		else if (btnName == "btn_buy")
		{
			const auto& items = DM->getData().items;
			auto itemIt = items.find(mSelectedItemID);
			if (itemIt != items.end())
			{
				const sItem& item = itemIt->second;
				if (PLAYER->spendGold(item.price))
				{
					PLAYER->addItem(mSelectedItemID);
					updatePlayerGold();
				}
			}
		}
	}
}