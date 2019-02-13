#include "CampScene.h"
#include "DataManager.h"
#include "MapScene.h"

CampScene::CampScene()
	: mGoldLabel(nullptr)
	, mCurrentVisibleLayer(nullptr) 
	, mUnknownIndex(-1) 
	, mSelectedTeammateIndex(mUnknownIndex)
	, mSelectedSlot(eSlotID::UNKNOWN_SLOT)
	, mSelectedItemIndex(mUnknownIndex)
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

bool CampScene::init()
{
	bool result = false;

	const std::string& view = DM->getViewById("CAMP_SCENE");

	if (Parent::init(view))
	{
		result = true;
		setButtonTouchListener(CC_CALLBACK_2(CampScene::onButtonTouched, this));

		mGoldLabel = static_cast<cocos2d::Label*>(findChildByName({ "bottom_bar", "gold", "text"}));
		updateGoldLabel();
		
		if (replaceVisibleLayer("teammate_layer"))
		{
			initTeammateLayer();
		}

		cocos2d::ui::Button* playerSlot = 
			static_cast<cocos2d::ui::Button*>(findChildByName({"player_layer", "player_damage", "left_slot"}));
		if (playerSlot != nullptr)
		{
			playerSlot->addTouchEventListener(CC_CALLBACK_2(CampScene::onPlayerSlotTouched, this));
		}

		playerSlot = static_cast<cocos2d::ui::Button*>(findChildByName({"player_layer", "player_damage", "right_slot"}));
		if (playerSlot != nullptr)
		{
			playerSlot->addTouchEventListener(CC_CALLBACK_2(CampScene::onPlayerSlotTouched, this));
		}
	} 
	return result;
}

void CampScene::updateGoldLabel()
{
	if (mGoldLabel != nullptr)
	{
		const std::string& goldStr = std::to_string(PLAYER->getGold());
		mGoldLabel->setString(goldStr);
	}
}

void CampScene::onButtonTouched(cocos2d::Ref* aSender, cocos2d::ui::Widget::TouchEventType aEvent)
{
	if (aEvent == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		cocos2d::ui::Button* btn = static_cast<cocos2d::ui::Button*>(aSender);
		const std::string& btnName = btn->getName();
		if (btnName == "player")
		{
			replaceVisibleLayer("player_layer");
			initPlayerLayer();
			initPlayerDamageLabels();
		}
		else if (btnName == "teammates")
		{
			if (replaceVisibleLayer("teammate_layer"))
			{
				initTeammateLayer();
			}	
		}
		else if (btnName == "inventory")
		{
			if(replaceVisibleLayer("inventory_layer"))
			{
				clearInventoryLayer();
				initInventoryLayer();
				setInventoryViewMode();
			}	
		}
		else if (btnName == "map")
		{
			cocos2d::Director::getInstance()->replaceScene(MapScene::createScene());
		}
		else if (btnName == "btn_equip")
		{
			if (mSelectedTeammateIndex != mUnknownIndex)
			{
				if (PLAYER->equipItem(mSelectedTeammateIndex, mSelectedSlot, mSelectedItemIndex))
				{
					if (replaceVisibleLayer("teammate_layer"))
					{
						initTeammateLayer();
					}
				}
			}
			else
			{
				if (PLAYER->equipItem(mSelectedSlot, mSelectedItemIndex))
				{
					if (replaceVisibleLayer("player_layer"))
					{
						initPlayerLayer();
						initPlayerDamageLabels();
					}
				}
			}

			mSelectedSlot = eSlotID::UNKNOWN_SLOT;
			mSelectedItemIndex = mUnknownIndex;
			mSelectedTeammateIndex = mUnknownIndex;
			
		}
		else if (btnName == "btn_clear")
		{
			if (mSelectedTeammateIndex != mUnknownIndex)
			{
				if (PLAYER->clearSlot(mSelectedTeammateIndex, mSelectedSlot))
				{
					if (replaceVisibleLayer("teammate_layer"))
					{
						initTeammateLayer();
					}
				}
			}
			else
			{
				if (PLAYER->clearSlot(mSelectedSlot))
				{
					if (replaceVisibleLayer("player_layer"))
					{
						initPlayerLayer();
						initPlayerDamageLabels();
					}
				}
			}
			mSelectedSlot = eSlotID::UNKNOWN_SLOT;
			mSelectedItemIndex = mUnknownIndex;
			mSelectedTeammateIndex = mUnknownIndex;	
		}
		else if (btnName == "btn_sell")
		{
			if (PLAYER->sellItem(mSelectedItemIndex))
			{
				cocos2d::Vec2 scrollviewPos;
				cocos2d::ui::ScrollView* scrollView = 
					static_cast<cocos2d::ui::ScrollView*>(findChildByName({ "inventory_layer", "list" }));
				if (scrollView != nullptr)
				{
					scrollviewPos = scrollView->getInnerContainerPosition();
				}

				clearInventoryLayer();
				initInventoryLayer();
				updateGoldLabel();
				
				if (scrollView != nullptr)
				{ 
					float maxPosY = -(scrollView->getInnerContainerSize().height - scrollView->getContentSize().height);
					if (scrollviewPos.y < maxPosY)
					{
						scrollviewPos.y = maxPosY;
					}
					scrollView->setInnerContainerPosition(scrollviewPos);
				}
			}
		}
	}
}

bool CampScene::replaceVisibleLayer(const std::string& aLayerName)
{
	cocos2d::Node* layer = getChildByName(aLayerName);
	if (layer != nullptr)
	{
		if (mCurrentVisibleLayer != nullptr)
		{
			mCurrentVisibleLayer->setVisible(false);
		}
		mCurrentVisibleLayer = layer;
		mCurrentVisibleLayer->setVisible(true);
	}

	return layer != nullptr;
}

void CampScene::initTeammateLayer()
{
	const std::string& templatePath = DM->getViewById("TEMPLATE_TEAMMATE_INVENTORY");
	cocos2d::Node* layer = getChildByName("teammate_layer");
	if (layer != nullptr)
	{
		cocos2d::ui::ScrollView* scrollView = layer->getChildByName<cocos2d::ui::ScrollView*>("list");
		if (scrollView != nullptr)
		{
			scrollView->removeAllChildren();
			cocos2d::Node* tmpTemplate = DM->createViewFromJson(templatePath);
			cocos2d::Size listSize = scrollView->getInnerContainerSize();
			const float templateHeight = tmpTemplate->getBoundingBox().size.height;
			const float templateHeightWithGap = templateHeight + (templateHeight * 0.1f);
			listSize.height = templateHeightWithGap * PLAYER->getTeam().size();
			scrollView->setInnerContainerSize(listSize);
			cocos2d::Vec2 templatePosition(listSize.width / 2.0f, listSize.height);

			for (unsigned int i = 0; i < PLAYER->getTeam().size(); ++i)
			{
				cocos2d::Node* templ = DM->createViewFromJson(templatePath);
				templ->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_TOP);
				templ->setPosition(templatePosition);
				scrollView->addChild(templ);
				templatePosition.y -= templateHeightWithGap;

				templ->setName(std::to_string(i));
				cocos2d::ui::Button* slot1 = templ->getChildByName<cocos2d::ui::Button*>("left_slot");
				const sTeammate& teammate = PLAYER->getTeam()[i];
				if (slot1 != nullptr)
				{
					slot1->addTouchEventListener(CC_CALLBACK_2(CampScene::onTeammateInventorySlotTouched, this));
					initTeammateSlotContent(teammate, slot1, eSlotID::LEFT_SLOT);

				}

				cocos2d::ui::Button* slot2 = templ->getChildByName<cocos2d::ui::Button*>("right_slot");
				if (slot2 != nullptr)
				{
					slot2->addTouchEventListener(CC_CALLBACK_2(CampScene::onTeammateInventorySlotTouched, this));
					initTeammateSlotContent(teammate, slot2, eSlotID::RIGHT_SLOT);
				}

				const sDamage& damage = PLAYER->getTeammateDamage(i);
				cocos2d::Node* phys_dmg = templ->getChildByName("phys_damage");
				if (phys_dmg != nullptr)
				{
					cocos2d::Label* text = phys_dmg->getChildByName<cocos2d::Label*>("text");
					if (text != nullptr)
					{
						text->setString(std::to_string(damage.physical));
					}
				}

				cocos2d::Node* mag_dmg = templ->getChildByName("mag_damage");
				if (mag_dmg != nullptr)
				{
					cocos2d::Label* text = mag_dmg->getChildByName<cocos2d::Label*>("text");
					if (text != nullptr)
					{
						text->setString(std::to_string(damage.magical));
					}
				}
			}
		}

		const sDamage& passiveDamage = PLAYER->getPassiveDamage();
		cocos2d::Label* physDamageLabel = static_cast<cocos2d::Label*>(findChildByName({
			"teammate_layer", "passive_damage", "phys_damage", "text"
		}));
		if (physDamageLabel != nullptr)
		{
			physDamageLabel->setString(std::to_string(passiveDamage.physical));
		}
		cocos2d::Label* magDamageLabel = static_cast<cocos2d::Label*>(findChildByName({
			"teammate_layer", "passive_damage", "mag_damage", "text"
		}));
		if (magDamageLabel != nullptr)
		{
			magDamageLabel->setString(std::to_string(passiveDamage.magical));
		}
	}
}

void CampScene::initTeammateSlotContent(const sTeammate& aTeammate, cocos2d::Node* aSlot, eSlotID aSlotID)
{
	auto slotIt = aTeammate.itemSlot.find(aSlotID);
	if (slotIt != aTeammate.itemSlot.end())
	{
		const std::string& itemID = slotIt->second;
		if (!itemID.empty())
		{
			auto itemIt = DM->getData().items.find(itemID);
			if (itemIt != DM->getData().items.end())
			{
				const std::string& frameNameID = itemIt->second.frameNameID;
				auto imageIt = DM->getData().images.find(frameNameID);
				if (imageIt != DM->getData().images.end())
				{
					const std::string& itemFrameName = imageIt->second;
					cocos2d::Sprite* itemSprite = cocos2d::Sprite::createWithSpriteFrameName(itemFrameName);
					if (itemSprite != nullptr)
					{
						cocos2d::Vec2 spritePos = aSlot->getBoundingBox().size / 2.0f;
						itemSprite->setPosition(spritePos);
						aSlot->addChild(itemSprite);
					}
				}
			}
		}
	}
}

void CampScene::onTeammateInventorySlotTouched(cocos2d::Ref* aSender, cocos2d::ui::Widget::TouchEventType aEvent)
{
	if (aEvent == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		cocos2d::Node* sender = static_cast<cocos2d::Node*>(aSender);
		const std::string& parent = sender->getParent()->getName();
		const std::string& slot = sender->getName();

		CCLOG("Teammate #%s, %s", parent.c_str(), slot.c_str());

		mSelectedTeammateIndex = std::stoi(parent.c_str());
		if (slot == "left_slot")
		{
			mSelectedSlot = eSlotID::LEFT_SLOT;
		}
		else if (slot == "right_slot")
		{
			mSelectedSlot = eSlotID::RIGHT_SLOT;
		}
		
		replaceVisibleLayer("inventory_layer");
		clearInventoryLayer();
		initInventoryLayer();
		setInventoryEquipMode();
		const std::string& equipedItem = PLAYER->getEquipedItem(mSelectedTeammateIndex, mSelectedSlot);
		initItemInfo(equipedItem);
	}
}

void CampScene::initInventoryLayer()
{

	const std::string& templatePath = DM->getViewById("TEMPLATE_INVENTORY_ROW");
	const std::vector<std::string>& inventory = PLAYER->getInventory();
	cocos2d::Label* msgEmpty = static_cast<cocos2d::Label*>(findChildByName({ "inventory_layer", "msg_empty"}));
	if (msgEmpty != nullptr)
	{
		msgEmpty->setVisible(inventory.empty());
	}
	cocos2d::ui::ScrollView* scrollView = static_cast<cocos2d::ui::ScrollView*>(findChildByName({"inventory_layer", "list"}));
	if (scrollView != nullptr)
	{
		cocos2d::Node* row = DM->createViewFromJson(templatePath);
		int itemsInRow = row->getChildrenCount();
		int numberOfRows = static_cast<int>(std::ceil(inventory.size() / static_cast<float>(itemsInRow)));
		cocos2d::Size scrollViewInnerSize = scrollView->getInnerContainerSize();
		int rowHeight = row->getBoundingBox().size.height;
		scrollViewInnerSize.height = numberOfRows * rowHeight;
		scrollView->setInnerContainerSize(scrollViewInnerSize);
		cocos2d::Vec2 rowPosition = scrollView->getInnerContainerSize();
		rowPosition.x /= 2.0f;

		unsigned int itemIndex = 0;
		for (int rowIndex = 0; rowIndex < numberOfRows; ++rowIndex)
		{
			for (auto child : row->getChildren())
			{
				if (itemIndex >= inventory.size())
				{
					break;
				}

				const std::string& itemID = inventory[itemIndex];
				const auto itemIt = DM->getData().items.find(itemID);
				if (itemIt != DM->getData().items.end())
				{
					const sItem& currentItem = itemIt->second;
					const auto frameNameIt = DM->getData().images.find(currentItem.frameNameID);
					if (frameNameIt != DM->getData().images.end())
					{
						const std::string& frameName = frameNameIt->second;
						cocos2d::Sprite* itemSprite = cocos2d::Sprite::createWithSpriteFrameName(frameName);
						cocos2d::Vec2 pos = child->getBoundingBox().size / 2.0f;
						itemSprite->setPosition(pos);
						child->addChild(itemSprite);
						child->setName(std::to_string(itemIndex));
						cocos2d::ui::Button* btn = static_cast<cocos2d::ui::Button*>(child);
						btn->addTouchEventListener(CC_CALLBACK_2(CampScene::onInventorySlotTouched, this));
					}
				}

				++itemIndex;
			}
			row->setPosition(rowPosition);
			scrollView->addChild(row);
			row = DM->createViewFromJson(templatePath);
			rowPosition.y -= rowHeight;
		}
	}
}

void CampScene::clearInventoryLayer()
{
	cocos2d::Node* layer = getChildByName("inventory_layer");
	if (layer != nullptr)
	{
		cocos2d::ui::ScrollView* scrollView = layer->getChildByName<cocos2d::ui::ScrollView*>("list");
		cocos2d::Sprite* itemPreview = layer->getChildByName<cocos2d::Sprite*>("item_preview");
		cocos2d::Label* itemTitle = layer->getChildByName<cocos2d::Label*>("item_title");
		cocos2d::Label* physDmgLabel = layer->getChildByName<cocos2d::Label*>("phys_dmg_label");
		cocos2d::Label* magDmgLabel = layer->getChildByName<cocos2d::Label*>("mag_dmg_label");

		if (scrollView && itemPreview && itemTitle && physDmgLabel && magDmgLabel)
		{
			scrollView->removeAllChildren();
			itemPreview->init();
			itemTitle->setString("");
			physDmgLabel->setString("");
			magDmgLabel->setString("");
		}
	}
}

void CampScene::setInventoryEquipMode()
{
	cocos2d::Node* layer = getChildByName("inventory_layer");
	if (layer != nullptr)
	{
		cocos2d::Node* btnEquip = layer->getChildByName("btn_equip");
		cocos2d::Node* btnClear = layer->getChildByName("btn_clear");
		cocos2d::Node* btnSell = layer->getChildByName("btn_sell");
		if (btnEquip && btnClear && btnSell)
		{
			btnEquip->setVisible(true);
			btnClear->setVisible(true);
			btnSell->setVisible(false);
		}
	}
}

void CampScene::setInventoryViewMode()
{
	cocos2d::Node* layer = getChildByName("inventory_layer");
	if (layer != nullptr)
	{
		cocos2d::Node* btnEquip = layer->getChildByName("btn_equip");
		cocos2d::Node* btnClear = layer->getChildByName("btn_clear");
		cocos2d::Node* btnSell = layer->getChildByName("btn_sell");
		if (btnEquip && btnClear && btnSell)
		{
			btnEquip->setVisible(false);
			btnClear->setVisible(false);
			btnSell->setVisible(true);
		}
	}
}

void CampScene::initItemInfo(const std::string& aItemID)
{
	const auto& items = DM->getData().items;
	auto itemIt = items.find(aItemID);
	if (itemIt != items.end())
	{
		const sItem& item = itemIt->second;
		cocos2d::Sprite* itemPreview = static_cast<cocos2d::Sprite*>(
			findChildByName({ "inventory_layer", "item_preview" }
		));
		cocos2d::Label* itemTitle = static_cast<cocos2d::Label*>(
			findChildByName({ "inventory_layer", "item_title" }
		));
		cocos2d::Label* physDmgLabel = static_cast<cocos2d::Label*>(
			findChildByName({ "inventory_layer", "phys_dmg_label" }
		));
		cocos2d::Label* magDmgLabel = static_cast<cocos2d::Label*>(
			findChildByName({ "inventory_layer", "mag_dmg_label" }
		));

		if (itemPreview && itemTitle && physDmgLabel && magDmgLabel)
		{
			const std::string& frameName = DM->getFrameNameById(item.frameNameID);
			itemPreview->initWithSpriteFrameName(frameName);

			const std::string itemName = DM->getStringById(item.name);
			itemTitle->setString(itemName);

			const std::string physDmgStr = std::to_string(item.damage.physical);
			physDmgLabel->setString(physDmgStr);
			const std::string magDmgStr = std::to_string(item.damage.magical);
			magDmgLabel->setString(magDmgStr);
		}
	}
}

void CampScene::onInventorySlotTouched(cocos2d::Ref* aSender, cocos2d::ui::Widget::TouchEventType aEvent)
{
	if (aEvent == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		cocos2d::Node* sender = static_cast<cocos2d::Node*>(aSender);
		mSelectedItemIndex = std::stoi(sender->getName().c_str());
		const std::string& itemID = PLAYER->getInventory()[mSelectedItemIndex];
		initItemInfo(itemID);
		cocos2d::ui::ScrollView* itemList = static_cast<cocos2d::ui::ScrollView*>(
			findChildByName({"inventory_layer", "list"})
		);
		if (itemList != nullptr)
		{
			const std::string selectedItemFrameName = "selected_item_frame";
			cocos2d::Sprite* selectedItemFrame = 
				static_cast<cocos2d::Sprite*>(itemList->getChildByName(selectedItemFrameName));
			if (selectedItemFrame == nullptr)
			{
				const auto& images = DM->getData().images;
				auto imageIt = images.find("SELECTED_ITEM_FRAME");
				if (imageIt != images.end())
				{
					selectedItemFrame = cocos2d::Sprite::createWithSpriteFrameName(imageIt->second);
					selectedItemFrame->setName(selectedItemFrameName);
					itemList->addChild(selectedItemFrame);
				}
			}
			if (selectedItemFrame != nullptr)
			{
				cocos2d::Node* senderParent = sender->getParent();
				if (senderParent != nullptr)
				{
					cocos2d::Vec2 framePosition;
					framePosition.x = senderParent->getBoundingBox().getMinX();
					framePosition.y = senderParent->getBoundingBox().getMinY();
					framePosition += sender->getPosition();
					selectedItemFrame->setPosition(framePosition);
				}	
			}
		}
	}
}

void CampScene::initPlayerLayer()
{
	cocos2d::Node* leftSlot = findChildByName({"player_layer", "player_damage", "left_slot"});
	if (leftSlot != nullptr)
	{
		initPlayerSlotContent(leftSlot, eSlotID::LEFT_SLOT);
	}
	cocos2d::Node* rightSlot = findChildByName({ "player_layer", "player_damage", "right_slot"});
	if (rightSlot != nullptr)
	{
		initPlayerSlotContent(rightSlot, eSlotID::RIGHT_SLOT);
	}
}

void CampScene::initPlayerSlotContent(cocos2d::Node* aSlot, eSlotID aSlotID)
{
	const std::string& equipedItem = PLAYER->getEquipedItem(aSlotID);
	if (!equipedItem.empty())
	{
		const auto& items = DM->getData().items;
		auto itemIt = items.find(equipedItem);
		if (itemIt != items.end())
		{
			const sItem& item = itemIt->second;
			const std::string& frameNameID = item.frameNameID;
			const std::string& frameName = DM->getFrameNameById(frameNameID);
			cocos2d::Sprite* itemSprite = cocos2d::Sprite::createWithSpriteFrameName(frameName);
			if (itemSprite != nullptr)
			{
				cocos2d::Vec2 spritePos = aSlot->getBoundingBox().size / 2.0f;
				itemSprite->setPosition(spritePos);
				aSlot->addChild(itemSprite);
			}
		}
	}
}

void CampScene::initPlayerDamageLabels()
{
	const sDamage& clickDamage = PLAYER->getClickDamage();

	cocos2d::Label* physDamageLabel = static_cast<cocos2d::Label*>(findChildByName({ 
		"player_layer", "player_damage", "phys_damage", "text"
	}));
	if (physDamageLabel != nullptr)
	{
		physDamageLabel->setString(std::to_string(clickDamage.physical));
	}

	cocos2d::Label* magDamageLabel = static_cast<cocos2d::Label*>(findChildByName({
		"player_layer", "player_damage", "mag_damage", "text"
	}));
	if (magDamageLabel != nullptr)
	{
		magDamageLabel->setString(std::to_string(clickDamage.magical));
	}
}

void CampScene::onPlayerSlotTouched(cocos2d::Ref* aSender, cocos2d::ui::Widget::TouchEventType aEvent)
{
	if (aEvent == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		cocos2d::Node* sender = static_cast<cocos2d::Node*>(aSender);
		const std::string& slot = sender->getName();
		sender->removeAllChildren();
		if (slot == "left_slot")
		{
			mSelectedSlot = eSlotID::LEFT_SLOT;
		}
		else if(slot == "right_slot")
		{
			mSelectedSlot = eSlotID::RIGHT_SLOT;
		}

		replaceVisibleLayer("inventory_layer");
		clearInventoryLayer();
		initInventoryLayer();
		setInventoryEquipMode();
		const std::string& equipedItem = PLAYER->getEquipedItem(mSelectedSlot);
		initItemInfo(equipedItem);
	}
}