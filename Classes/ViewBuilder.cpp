#include "ViewBuilder.h"
#include "viewObjectType.h"
#include "json/document.h"
#include "DataManager.h"

ViewBuilder::ViewBuilder()
{
	mComponents["node"] = new NodeComponent(mComponents);
	mComponents["sprite"] = new SpriteComponent(mComponents);
	mComponents["label"] = new LabelComponent(mComponents);
	mComponents["button"] = new ButtonComponent(mComponents);
	mComponents["pop_up_layer"] = new PopUpLayerComponent(mComponents);
	mComponents["map_scrollview"] = new MapScrollViewComponent(mComponents);
	mComponents["loading_bar"] = new LoadingBarComponent(mComponents);
}

ViewBuilder::~ViewBuilder()
{
	for (auto component : mComponents)
	{
		delete component.second;
	}
}

bool ViewBuilder::loadFromJson(cocos2d::Node* aParent, const std::string& aJson)
{
	rapidjson::Document json;
	const std::string content = cocos2d::FileUtils::getInstance()->getStringFromFile(aJson);
	json.Parse(content.c_str());
	bool result = false;

	if (!json.HasParseError())
	{
		auto componentIt = mComponents.find("node");
		if (componentIt != mComponents.end())
		{
			for (auto it = json.MemberBegin(); it != json.MemberEnd(); ++it)
			{
				componentIt->second->init(nullptr, aParent, it);
			}
		}
		result = true;
	}
	else
	{
		CCLOG("'%s' parsing error", aJson.c_str());
	}

	return result;
}

ViewBuilder::ViewComponent::ViewComponent(
	const std::map<std::string, ViewComponent*>& aComponents)
	: mComponents(aComponents)
{
}

cocos2d::Color3B ViewBuilder::ViewComponent::getColorFromValue(const rapidjson::Value& aAttrIt)
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

ViewBuilder::NodeComponent::NodeComponent(
	const std::map<std::string, ViewComponent*>& aComponents)
	: ViewComponent(aComponents)
{
}

cocos2d::Node* ViewBuilder::NodeComponent::create(const rapidjson::Value& aAttr)
{
	return cocos2d::Node::create();
}

bool ViewBuilder::NodeComponent::init(
	const cocos2d::Node* aParent, 
	cocos2d::Node* aObject, 
	rapidjson::Value::MemberIterator aAttrIt)
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
	else if (attrName == "z_order")
	{
		int zOrder = aAttrIt->value.GetInt();
		aObject->setLocalZOrder(zOrder);
	}
	else if (attrName == "children")
	{
		loadChildren(aObject, aAttrIt->value);
		result = true;
	}

	return result;
}

void ViewBuilder::NodeComponent::addToParent(cocos2d::Node* aParent, cocos2d::Node* aChild)
{
	aParent->addChild(aChild);
}

cocos2d::Node* ViewBuilder::NodeComponent::initChild(
	cocos2d::Node* aParent, rapidjson::Value::MemberIterator aChildIt)
{
	cocos2d::Node* result = nullptr;
	std::string childType = aChildIt->name.GetString();
	auto componentIt = mComponents.find(childType);
	if (componentIt != mComponents.end())
	{
		cocos2d::Node* child = componentIt->second->create(aChildIt->value);
		if (child != nullptr)
		{
			for (auto attrIt = aChildIt->value.MemberBegin(); attrIt != aChildIt->value.MemberEnd(); ++attrIt)
			{
				componentIt->second->init(aParent, child, attrIt);
			}
			result = child;
		}
	}
	return result;
}

void ViewBuilder::NodeComponent::loadChildren(
	cocos2d::Node* aParent, rapidjson::Value& aChildrenValue)
{
	for (auto it = aChildrenValue.MemberBegin(); it != aChildrenValue.MemberEnd(); ++it)
	{
		cocos2d::Node* child = initChild(aParent, it);
		if (child != nullptr)
		{
			aParent->addChild(child);
		}
	}
}

ViewBuilder::SpriteComponent::SpriteComponent(
	const std::map<std::string, ViewComponent*>& aComponents)
	: NodeComponent(aComponents)
{
}

cocos2d::Node* ViewBuilder::SpriteComponent::create(const rapidjson::Value& aAttr)
{
	return cocos2d::Sprite::create();
}

bool ViewBuilder::SpriteComponent::init(
	const cocos2d::Node* aParent, cocos2d::Node* aObject, 
	rapidjson::Value::MemberIterator aAttrIt)
{
	bool result = false;

	if (!NodeComponent::init(aParent, aObject, aAttrIt))
	{
		const std::string attrName = aAttrIt->name.GetString();
		cocos2d::Sprite* sprite = static_cast<cocos2d::Sprite*>(aObject);
		if (attrName == "frame_name")
		{
			const auto frameName = DM->getData().images.find(aAttrIt->value.GetString());
			if (frameName != DM->getData().images.end())
			{
				sprite->initWithSpriteFrameName(frameName->second);
				result = true;
			}
		}
	}

	return result;
}

ViewBuilder::LabelComponent::LabelComponent(const std::map<std::string, ViewComponent*>& aComponents)
	:NodeComponent(aComponents)
{
}

cocos2d::Node* ViewBuilder::LabelComponent::create(const rapidjson::Value& aAttr)
{
	cocos2d::Label* label = nullptr;

	if (aAttr.HasMember("font"))
	{
		std::string fontID = aAttr["font"].GetString();
		const std::string& font = DM->getFontById(fontID);

		if (aAttr.HasMember("text"))
		{
			std::string stringID = aAttr["text"].GetString();
			const std::string& text = DM->getStringById(stringID);

			label = cocos2d::Label::createWithBMFont(font, text);
		}
		else
		{
			label = cocos2d::Label::createWithBMFont(font, "");
		}
	}

	return label;
}

bool ViewBuilder::LabelComponent::init(
	const cocos2d::Node* aParent, 
	cocos2d::Node* aObject, 
	rapidjson::Value::MemberIterator aAttrIt)
{
	bool result = false;

	if (!NodeComponent::init(aParent, aObject, aAttrIt))
	{
		const std::string attrName = aAttrIt->name.GetString();
		cocos2d::Label* label = static_cast<cocos2d::Label*>(aObject);
		if (attrName == "width")
		{
			float width = static_cast<float>(aAttrIt->value.GetDouble());
			width *= aParent->getContentSize().width;
			label->setMaxLineWidth(width);
			result = true;
		}
	}

	return result;
}

ViewBuilder::ButtonComponent::ButtonComponent(
	const std::map<std::string, ViewComponent*>& aComponents)
	: NodeComponent(aComponents)
{
}

cocos2d::Node* ViewBuilder::ButtonComponent::create(const rapidjson::Value& aAttr)
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

bool ViewBuilder::ButtonComponent::init(
	const cocos2d::Node* aParent, 
	cocos2d::Node* aObject, 
	rapidjson::Value::MemberIterator aAttrIt)
{
	bool result = false;
	aObject->setTag(static_cast<int>(eViewObjectType::OBJECT_TYPE_BUTTON));
	if (!NodeComponent::init(aParent, aObject, aAttrIt))
	{
		const std::string attrName = aAttrIt->name.GetString();
		cocos2d::ui::Button* button = static_cast<cocos2d::ui::Button*>(aObject);
		if (attrName == "title")
		{
			auto componentIt = mComponents.find("label");
			if (componentIt != mComponents.end())
			{
				cocos2d::Node* titleNode = componentIt->second->create(aAttrIt->value);
				cocos2d::Label* title = static_cast<cocos2d::Label*>(titleNode);
				if (title != nullptr)
				{
					title->setPosition(cocos2d::Vec2::ANCHOR_MIDDLE);
					button->setTitleLabel(title);
				}
				result = true;
			}
		}
	}

	return result;
}

ViewBuilder::PopUpLayerComponent::PopUpLayerComponent(
	const std::map<std::string, ViewComponent*>& aComponents)
	: NodeComponent(aComponents)
{
}

cocos2d::Node* ViewBuilder::PopUpLayerComponent::create(const rapidjson::Value& aAttr)
{
	return PopUpLayer::create();
}

bool ViewBuilder::PopUpLayerComponent::init(
	const cocos2d::Node* aParent,
	cocos2d::Node* aObject, 
	rapidjson::Value::MemberIterator aAttrIt)
{
	bool result = false;
	
	if (!NodeComponent::init(aParent, aObject, aAttrIt))
	{
		const std::string attrName = aAttrIt->name.GetString();
		PopUpLayer* layer = static_cast<PopUpLayer*>(aObject);
		if (attrName == "appearingTime")
		{
			layer->setAppeatingTime(static_cast<float>(aAttrIt->value.GetDouble()));
			result = true;
		}
	}

	return result;
}

ViewBuilder::MapScrollViewComponent::MapScrollViewComponent(
	const std::map<std::string, ViewComponent*>& aComponents)
	: NodeComponent(aComponents)
{
}

cocos2d::Node* ViewBuilder::MapScrollViewComponent::create(
	const rapidjson::Value& aAttr)
{
	return cocos2d::ui::ScrollView::create();
}

bool ViewBuilder::MapScrollViewComponent::init(
	const cocos2d::Node* aParent, 
	cocos2d::Node* aObject, 
	rapidjson::Value::MemberIterator aAttrIt)
{
	bool result = false;

	if (!NodeComponent::init(aParent, aObject, aAttrIt))
	{
		const std::string attrName = aAttrIt->name.GetString();
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
			loadParts(static_cast<cocos2d::ui::ScrollView*>(aObject), aAttrIt->value);
		}
	}

	cocos2d::ui::ScrollView* scrollView = static_cast<cocos2d::ui::ScrollView*>(aObject);
	scrollView->setScrollBarEnabled(false);
	scrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
	scrollView->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);

	return result;
}

void ViewBuilder::MapScrollViewComponent::loadParts(
	cocos2d::ui::ScrollView* aParent, rapidjson::Value& aPartsValue)
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

ViewBuilder::LoadingBarComponent::LoadingBarComponent(
	const std::map<std::string, ViewComponent*>& aComponents)
	: NodeComponent(aComponents)
{
}

cocos2d::Node* ViewBuilder::LoadingBarComponent::create(const rapidjson::Value& aAttr)
{
	cocos2d::Node* result = nullptr;

	if (aAttr.HasMember("core_frame_name"))
	{
		const std::string frameNameID = aAttr["core_frame_name"].GetString();
		const std::string& frameName = DM->getFrameNameById(frameNameID);
		result = cocos2d::ui::LoadingBar::create(frameName, cocos2d::ui::Widget::TextureResType::PLIST);
	}

	return result;
}