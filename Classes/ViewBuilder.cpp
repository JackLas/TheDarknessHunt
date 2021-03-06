#include "ViewBuilder.h"
#include "viewObjectType.h"
#include "json/document.h"
#include "DataManager.h"

ViewBuilder::ViewBuilder()
{
	mComponents["node"] = new NodeComponent(mComponents);
	mComponents["layer"] = new LayerComponent(mComponents);
	mComponents["sprite"] = new SpriteComponent(mComponents);
	mComponents["label"] = new LabelComponent(mComponents);
	mComponents["button"] = new ButtonComponent(mComponents);
	mComponents["pop_up_layer"] = new PopUpLayerComponent(mComponents);
	mComponents["scrollview"] = new ScrollViewComponent(mComponents);
	mComponents["map_scrollview"] = new MapScrollViewComponent(mComponents);
	mComponents["loading_bar"] = new LoadingBarComponent(mComponents);
	mComponents["view"] = new ViewObjectComponent(mComponents);
	mComponents["label_button"] = new LabelButtonComponent(mComponents);
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

cocos2d::Node* ViewBuilder::createViewFromJson(const std::string& aJson)
{
	rapidjson::Document json;
	const std::string content = cocos2d::FileUtils::getInstance()->getStringFromFile(aJson);
	json.Parse(content.c_str());
	cocos2d::Node* result = nullptr;

	if (!json.HasParseError())
	{
		std::string type = "node";
		if (json.HasMember("type"))
		{
			type = json["type"].GetString();
		}

		auto componentIt = mComponents.find(type);
		if (componentIt != mComponents.end())
		{
			ViewComponent* component = componentIt->second;
			result = component->create(json);

			for (auto it = json.MemberBegin(); it != json.MemberEnd(); ++it)
			{
				componentIt->second->init(nullptr, result, it);
			}
		}
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

cocos2d::Color3B ViewBuilder::ViewComponent::getColorFromValue(const rapidjson::Value& aAttrIt) const
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

cocos2d::Node* ViewBuilder::NodeComponent::create(const rapidjson::Value& aAttr) const
{
	return cocos2d::Node::create();
}

bool ViewBuilder::NodeComponent::init(
	const cocos2d::Node* aParent, 
	cocos2d::Node* aObject, 
	rapidjson::Value::MemberIterator aAttrIt) const
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
		result = true;
	}
	else if (attrName == "color")
	{
		cocos2d::Color3B color = getColorFromValue(aAttrIt->value);
		aObject->setColor(color);
		result = true;
	}
	else if (attrName == "anchor_x")
	{
		cocos2d::Vec2 anchor = aObject->getAnchorPoint();
		anchor.x = static_cast<float>(aAttrIt->value.GetDouble());
		aObject->setAnchorPoint(anchor);
		result = true;
	}
	else if (attrName == "anchor_y")
	{
		cocos2d::Vec2 anchor = aObject->getAnchorPoint();
		anchor.y = static_cast<float>(aAttrIt->value.GetDouble());
		aObject->setAnchorPoint(anchor);
		result = true;
	}
	else if (attrName == "z_order")
	{
		int zOrder = aAttrIt->value.GetInt();
		aObject->setLocalZOrder(zOrder);
		result = true;
	}
	else if (attrName == "opacity")
	{
		int configOpacity = aAttrIt->value.GetInt();
		unsigned char opacity = 0;
		if (opacity < 0)
		{
			opacity = 0;
		}
		else if (opacity > 256)
		{
			opacity = 255;
		}
		else
		{
			opacity = static_cast<unsigned char>(configOpacity);
		}
		aObject->setOpacity(opacity);
	}
	else if (attrName == "children")
	{
		loadChildren(aObject, aAttrIt->value);
		result = true;
	}

	return result;
}

void ViewBuilder::NodeComponent::addToParent(cocos2d::Node* aParent, cocos2d::Node* aChild) const
{
	aParent->addChild(aChild);
}

cocos2d::Node* ViewBuilder::NodeComponent::initChild(
	cocos2d::Node* aParent, rapidjson::Value::MemberIterator aChildIt, const ViewComponent* aComponent) const
{
	cocos2d::Node* child = aComponent->create(aChildIt->value);
	if (child != nullptr)
	{
		for (auto attrIt = aChildIt->value.MemberBegin(); attrIt != aChildIt->value.MemberEnd(); ++attrIt)
		{
			aComponent->init(aParent, child, attrIt);
		}
	}
	return child;
}

void ViewBuilder::NodeComponent::loadChildren(
	cocos2d::Node* aParent, rapidjson::Value& aChildrenValue) const
{
	for (auto it = aChildrenValue.MemberBegin(); it != aChildrenValue.MemberEnd(); ++it)
	{
		std::string childType = it->name.GetString();
		auto componentIt = mComponents.find(childType);
		if (componentIt != mComponents.end())
		{
			cocos2d::Node* child = initChild(aParent, it, componentIt->second);
			if (child != nullptr)
			{
				componentIt->second->addToParent(aParent, child);
				//aParent->addChild(child);
			}
		}
	}
}

ViewBuilder::LayerComponent::LayerComponent(
	const std::map<std::string, ViewComponent*>& aComponents)
	: NodeComponent(aComponents)
{
}

cocos2d::Node* ViewBuilder::LayerComponent::create(const rapidjson::Value& aAttr) const
{
	return cocos2d::Layer::create();
}

ViewBuilder::SpriteComponent::SpriteComponent(
	const std::map<std::string, ViewComponent*>& aComponents)
	: NodeComponent(aComponents)
{
}

cocos2d::Node* ViewBuilder::SpriteComponent::create(const rapidjson::Value& aAttr) const
{
	return cocos2d::Sprite::create();
}

bool ViewBuilder::SpriteComponent::init(
	const cocos2d::Node* aParent, cocos2d::Node* aObject, 
	rapidjson::Value::MemberIterator aAttrIt) const
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
		else if (attrName == "flip_x")
		{
			sprite->setFlippedX(aAttrIt->value.GetBool());
			result = true;
		}
		else if (attrName == "flip_y")
		{
			sprite->setFlippedY(aAttrIt->value.GetBool());
			result = true;
		}
	}

	return result;
}

ViewBuilder::LabelComponent::LabelComponent(const std::map<std::string, ViewComponent*>& aComponents)
	:NodeComponent(aComponents)
{
}

cocos2d::Node* ViewBuilder::LabelComponent::create(const rapidjson::Value& aAttr) const
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
	rapidjson::Value::MemberIterator aAttrIt) const
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

cocos2d::Node* ViewBuilder::ButtonComponent::create(const rapidjson::Value& aAttr) const
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
	rapidjson::Value::MemberIterator aAttrIt) const
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

cocos2d::Node* ViewBuilder::PopUpLayerComponent::create(const rapidjson::Value& aAttr) const
{
	return PopUpLayer::create();
}

bool ViewBuilder::PopUpLayerComponent::init(
	const cocos2d::Node* aParent,
	cocos2d::Node* aObject, 
	rapidjson::Value::MemberIterator aAttrIt) const
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

ViewBuilder::ScrollViewComponent::ScrollViewComponent(
	const std::map<std::string, ViewComponent*>& aComponents)
	: NodeComponent(aComponents)
{

}

cocos2d::Node* ViewBuilder::ScrollViewComponent::create(
	const rapidjson::Value& aAttr) const
{
	cocos2d::ui::ScrollView* scrollview = cocos2d::ui::ScrollView::create();
	scrollview->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	return scrollview;
}

bool ViewBuilder::ScrollViewComponent::init(const cocos2d::Node* aParent,
	cocos2d::Node* aObject,
	rapidjson::Value::MemberIterator aAttrIt) const
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
			result = true;
		}
		else if (attrName == "content_height")
		{
			cocos2d::Size currentSize = aObject->getContentSize();
			currentSize.height = aParent->getContentSize().height * static_cast<float>(aAttrIt->value.GetDouble());
			aObject->setContentSize(currentSize);
			result = true;
		}
		else if (attrName == "scroll_bar_enabled")
		{
			cocos2d::ui::ScrollView* scrollview = static_cast<cocos2d::ui::ScrollView*>(aObject);
			bool state = aAttrIt->value.GetBool();
			scrollview->setScrollBarEnabled(state);
			result = true;
		}
	}

	return result;
}

ViewBuilder::MapScrollViewComponent::MapScrollViewComponent(
	const std::map<std::string, ViewComponent*>& aComponents)
	: ScrollViewComponent(aComponents)
{
}

bool ViewBuilder::MapScrollViewComponent::init(
	const cocos2d::Node* aParent, 
	cocos2d::Node* aObject, 
	rapidjson::Value::MemberIterator aAttrIt) const
{
	bool result = false;

	if (!ScrollViewComponent::init(aParent, aObject, aAttrIt))
	{
		const std::string attrName = aAttrIt->name.GetString();
		if (attrName == "parts")
		{
			loadParts(static_cast<cocos2d::ui::ScrollView*>(aObject), aAttrIt->value);
			result = true;
		}
	}

	cocos2d::ui::ScrollView* scrollView = static_cast<cocos2d::ui::ScrollView*>(aObject);
	scrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);


	return result;
}

void ViewBuilder::MapScrollViewComponent::loadParts(
	cocos2d::ui::ScrollView* aParent, rapidjson::Value& aPartsValue) const
{
	const cocos2d::Size& contentSize = aParent->getContentSize();
	cocos2d::Size innerContainerSize = aParent->getInnerContainerSize();
	innerContainerSize.height = 0.0f;

	for (auto it = aPartsValue.MemberBegin(); it != aPartsValue.MemberEnd(); ++it)
	{
		std::string childType = it->name.GetString();
		auto componentIt = mComponents.find(childType);
		if (componentIt != mComponents.end())
		{
			cocos2d::Node* child = initChild(aParent, it, componentIt->second);
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

	}
	aParent->setInnerContainerSize(innerContainerSize);
}

ViewBuilder::LoadingBarComponent::LoadingBarComponent(
	const std::map<std::string, ViewComponent*>& aComponents)
	: NodeComponent(aComponents)
{
}

cocos2d::Node* ViewBuilder::LoadingBarComponent::create(const rapidjson::Value& aAttr) const
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

ViewBuilder::ViewObjectComponent::ViewObjectComponent(
	const std::map<std::string, ViewComponent*>& aComponents)
	: NodeComponent(aComponents)
{
}

cocos2d::Node* ViewBuilder::ViewObjectComponent::create(const rapidjson::Value& aAttr) const
{
	cocos2d::Node* object = nullptr;
	if (aAttr.HasMember("view_id"))
	{
		const std::string viewID = aAttr["view_id"].GetString();
		auto viewIt = DM->getData().views.find(viewID);
		if (viewIt != DM->getData().views.end())
		{
			const std::string& viewPath = viewIt->second;
			object = DM->createViewFromJson(viewPath);
		}
	}
	return object;
}

ViewBuilder::LabelButtonComponent::LabelButtonComponent(
	const std::map<std::string, ViewComponent*>& aComponents)
	: LabelComponent(aComponents)
{
}

void ViewBuilder::LabelButtonComponent::addToParent(cocos2d::Node* aParent, cocos2d::Node* aChild) const
{
	cocos2d::ui::Button* btn = cocos2d::ui::Button::create();
	if (btn != nullptr)
	{
		btn->setTag(static_cast<int>(eViewObjectType::OBJECT_TYPE_BUTTON));
		btn->setName(aChild->getName());
		btn->setAnchorPoint(aChild->getAnchorPoint());
		aChild->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
		btn->setPosition(aChild->getPosition());
		aChild->setPosition(cocos2d::Vec2::ZERO);
		btn->setLocalZOrder(aChild->getLocalZOrder());
		aChild->setLocalZOrder(0);
		btn->setVisible(aChild->isVisible());
		aChild->setVisible(true);
		btn->setTitleLabel(static_cast<cocos2d::Label*>(aChild));	
		aParent->addChild(btn);
	}
}