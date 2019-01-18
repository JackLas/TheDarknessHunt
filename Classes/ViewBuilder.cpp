#include "ViewBuilder.h"
#include "viewObjectType.h"

#include "json/document.h"
#include "DataManager.h"

bool ViewBuilder::loadFromJson(cocos2d::Node* aParent, const std::string& aJson)
{
	rapidjson::Document json;
	const std::string content = cocos2d::FileUtils::getInstance()->getStringFromFile(aJson);
	json.Parse(content.c_str());
	bool result = false;

	if (!json.HasParseError())
	{
		for (auto it = json.MemberBegin(); it != json.MemberEnd(); ++it)
		{
			initNode(nullptr, aParent, it);
		}
		result = true;
	}
	else
	{
		result = false;
		CCLOG("'%s' parsing error", aJson.c_str());
	}

	return result;
}

cocos2d::Node* ViewBuilder::initChild(cocos2d::Node* aParent, rapidjson::Value::MemberIterator aChildIt)
{
	const std::string childType = aChildIt->name.GetString();
	rapidjson::Value& attributes = aChildIt->value;
	cocos2d::Node* result = nullptr;

	if (childType == "sprite")
	{
		cocos2d::Sprite* sprite = cocos2d::Sprite::create();
		sprite->setTag(static_cast<int>(eViewObjectType::OBJECT_TYPE_SPRITE));
		if (sprite)
		{
			for (auto attrIt = attributes.MemberBegin(); attrIt != attributes.MemberEnd(); ++attrIt)
			{
				if (initNode(aParent, sprite, attrIt))
					;
				else if (initSprite(aParent, sprite, attrIt))
					;
			}
			result = sprite;
		}
	}
	else if (childType == "button")
	{
		cocos2d::ui::Button* btn = createButton(attributes);
		btn->setTag(static_cast<int>(eViewObjectType::OBJECT_TYPE_BUTTON));
		if (btn)
		{
			for (auto attrIt = attributes.MemberBegin(); attrIt != attributes.MemberEnd(); ++attrIt)
			{
				if (initButton(aParent, btn, attrIt))
					;
				else if (initNode(aParent, btn, attrIt))
					;

			}
			result = btn;
		}
	}
	else if (childType == "label")
	{
		cocos2d::Label* label = createLabel(attributes);
		label->setTag(static_cast<int>(eViewObjectType::OBJECT_TYPE_LABEL));
		if (label != nullptr)
		{
			for (auto attrIt = attributes.MemberBegin(); attrIt != attributes.MemberEnd(); ++attrIt)
			{
				if (initLabel(aParent, label, attrIt))
					;
				else if (initNode(aParent, label, attrIt))
					;
			}
			result = label;
		}
	}
	else if (childType == "pop_up_layer")
	{
		PopUpLayer* layer = PopUpLayer::create();
		layer->setTag(static_cast<int>(eViewObjectType::OBJECT_TYPE_POP_UP_LAYER));
		if (layer != nullptr)
		{
			for (auto attrIt = attributes.MemberBegin(); attrIt != attributes.MemberEnd(); ++attrIt)
			{
				if (initPopUpLayer(aParent, layer, attrIt))
					;
				else if (initNode(aParent, layer, attrIt))
					;
			}
			result = layer;
		}
	}
	else if (childType == "scrollview")
	{
		cocos2d::ui::ScrollView* scrollView = cocos2d::ui::ScrollView::create();
		if (scrollView != nullptr)
		{
			for (auto attrIt = attributes.MemberBegin(); attrIt != attributes.MemberEnd(); ++attrIt)
			{
				if (initScrollView(aParent, scrollView, attrIt))
					;
				else if (initNode(aParent, scrollView, attrIt))
					;
			}
			scrollView->setScrollBarEnabled(false);
			scrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
			scrollView->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);

			result = scrollView;
		}
	}

	return result;
}

void ViewBuilder::loadChildren(cocos2d::Node* aParent, rapidjson::Value& aChildrenValue)
{
	const cocos2d::Size& contentSize = aParent->getContentSize();

	for (auto it = aChildrenValue.MemberBegin(); it != aChildrenValue.MemberEnd(); ++it)
	{
		cocos2d::Node* child = initChild(aParent, it);
		if (child != nullptr)
		{
			aParent->addChild(child);
		}
	}
}

void ViewBuilder::loadScrollViewParts(cocos2d::ui::ScrollView* aParent, rapidjson::Value& aPartsValue)
{
	const cocos2d::Size& contentSize = aParent->getContentSize();
	cocos2d::Size innerContainerSize = aParent->getInnerContainerSize();
	innerContainerSize.height = 0.0f;

	for (auto it = aPartsValue.MemberBegin(); it != aPartsValue.MemberEnd(); ++it)
	{
		cocos2d::Node* child = initChild(aParent, it);
		if (child != nullptr)
		{
			child->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_BOTTOM);
			const cocos2d::Size& childSize = child->getContentSize();
			cocos2d::Size childPos = innerContainerSize;
			childPos.width *= 0.5f;
			child->setPosition(childPos);
			innerContainerSize.height += childSize.height;
			aParent->addChild(child);
		}
	}
	aParent->setInnerContainerSize(innerContainerSize);

}

bool ViewBuilder::initNode(const cocos2d::Node* aParent, cocos2d::Node* aObject, rapidjson::Value::MemberIterator aAttrIt)
{
	cocos2d::Size contentSize;
	if (aParent != nullptr)
	{
		contentSize = aParent->getContentSize();
	}
	
	const std::string attrName = aAttrIt->name.GetString();
	bool result = false;
	
	if (attrName == "name")
	{
		const std::string name = aAttrIt->value.GetString();
		aObject->setName(name);
		result = true;
	}
	else if (attrName == "pos_x")
	{
		float posX = static_cast<float>(aAttrIt->value.GetDouble());
		aObject->setPositionX(contentSize.width * posX);
		result = true;
	}
	else if (attrName == "pos_y")
	{
		float posY = static_cast<float>(aAttrIt->value.GetDouble());
		aObject->setPositionY(contentSize.height * posY);
		result = true;
	}
	else if (attrName == "visible")
	{
		aObject->setVisible(aAttrIt->value.GetBool());
	}
	else if (attrName == "color")
	{
		cocos2d::Color3B color = getColorFromValue(aAttrIt->value);
		aObject->setColor(color);
	}
	else if (attrName == "anchor_x")
	{
		cocos2d::Vec2 anchor = aObject->getAnchorPoint();
		anchor.x = static_cast<float>(aAttrIt->value.GetDouble());
		aObject->setAnchorPoint(anchor);
	}
	else if (attrName == "anchor_y")
	{
		cocos2d::Vec2 anchor = aObject->getAnchorPoint();
		anchor.y = static_cast<float>(aAttrIt->value.GetDouble());
		aObject->setAnchorPoint(anchor);
	}
	else if (attrName == "children")
	{
		loadChildren(aObject, aAttrIt->value);
		result = true;
	}

	return result;
}

bool ViewBuilder::initButton(const cocos2d::Node* aParent, cocos2d::ui::Button* aObject, rapidjson::Value::MemberIterator aAttrIt)
{
	bool result = false;
	const std::string attrName = aAttrIt->name.GetString();

	if (attrName == "title")
	{
		cocos2d::Label* title = createLabel(aAttrIt->value);
		if (title != nullptr)
		{
			title->setPosition(cocos2d::Vec2::ANCHOR_MIDDLE);
			aObject->setTitleLabel(title);
		}
		result = true;
	}

	return result;
}

bool ViewBuilder::initSprite(const cocos2d::Node* aParent, cocos2d::Sprite* aObject, rapidjson::Value::MemberIterator aAttrIt)
{
	bool result = false;
	const std::string attrName = aAttrIt->name.GetString();

	if (attrName == "frame_name")
	{
		const auto frameName = DM->getData().images.find(aAttrIt->value.GetString());
		if (frameName != DM->getData().images.end())
		{
			aObject->initWithSpriteFrameName(frameName->second);
			result = true;
		}
	}

	return result;
}

bool ViewBuilder::initLabel(const cocos2d::Node* aParent, cocos2d::Label* aObject, rapidjson::Value::MemberIterator aAttrIt)
{
	bool result = false;
	const std::string attrName = aAttrIt->name.GetString();
	if (attrName == "width")
	{
		float width = static_cast<float>(aAttrIt->value.GetDouble());
		width *= aParent->getContentSize().width;
		aObject->setMaxLineWidth(width);
	}

	return result;
}

bool ViewBuilder::initPopUpLayer(const cocos2d::Node* aParent, PopUpLayer* aObject, rapidjson::Value::MemberIterator aAttrIt)
{
	const std::string attrName = aAttrIt->name.GetString();
	bool result = false;

	if (attrName == "appearingTime")
	{
		aObject->setAppeatingTime(static_cast<float>(aAttrIt->value.GetDouble()));
		result = true;
	}

	return result;
}

bool ViewBuilder::initScrollView(const cocos2d::Node* aParent, cocos2d::ui::ScrollView* aObject, rapidjson::Value::MemberIterator aAttrIt)
{
	const std::string attrName = aAttrIt->name.GetString();
	bool result = false;

	if (attrName == "content_width")
	{
		cocos2d::Size currentSize = aObject->getContentSize();
		currentSize.width = aParent->getContentSize().width * static_cast<float>(aAttrIt->value.GetDouble());
		aObject->setContentSize(currentSize);
	}
	else if (attrName == "content_height")
	{
		cocos2d::Size currentSize = aObject->getContentSize();
		currentSize.height = aParent->getContentSize().height * static_cast<float>(aAttrIt->value.GetDouble());
		aObject->setContentSize(currentSize);
	}
	else if (attrName == "parts")
	{
		loadScrollViewParts(static_cast<cocos2d::ui::ScrollView*>(aObject), aAttrIt->value);
	}

	return result;
}

cocos2d::ui::Button* ViewBuilder::createButton(const rapidjson::Value& aAttr)
{
	std::string normalFrameName;
	if (aAttr.HasMember("normal_image_frame_name"))
	{
		normalFrameName = DM->getFrameNameById(aAttr["normal_image_frame_name"].GetString());
	}

	std::string selectedFrameName;
	if (aAttr.HasMember("selected_image_frame_name"))
	{
		selectedFrameName = DM->getFrameNameById(aAttr["selected_image_frame_name"].GetString());
	}
	std::string disabledFrameName;
	if (aAttr.HasMember("disabled_image_frame_name"))
	{
		disabledFrameName = DM->getFrameNameById(aAttr["disabled_image_frame_name"].GetString());
	}

	return cocos2d::ui::Button::create(	normalFrameName,
										selectedFrameName,
										disabledFrameName,
										cocos2d::ui::Widget::TextureResType::PLIST);
}

cocos2d::Label* ViewBuilder::createLabel(const rapidjson::Value& aAttr)
{
	cocos2d::Label* label = nullptr;

	if (aAttr.HasMember("font") && aAttr.HasMember("text"))
	{
		std::string fontID = aAttr["font"].GetString();
		const std::string& font = DM->getFontById(fontID);

		std::string stringID = aAttr["text"].GetString();
		const std::string& text = DM->getStringById(stringID);

		label = cocos2d::Label::createWithBMFont(font, text);
	}

	return label;
}

cocos2d::Color3B ViewBuilder::getColorFromValue(const rapidjson::Value& aAttrIt)
{
	cocos2d::Color3B color = cocos2d::Color3B::WHITE;

	if (aAttrIt.HasMember("r"))
	{
		color.r = static_cast<float>(aAttrIt["r"].GetDouble());
	}
	if (aAttrIt.HasMember("g"))
	{
		color.g = static_cast<float>(aAttrIt["g"].GetDouble());
	}
	if (aAttrIt.HasMember("b"))
	{
		color.b = static_cast<float>(aAttrIt["b"].GetDouble());
	}

	return color;
}