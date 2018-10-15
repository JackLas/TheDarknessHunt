#include "ViewBuilder.h"
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
		std::string type;
		if (json.HasMember("type"))
		{
			type = json["type"].GetString();
		}

		for (auto it = json.MemberBegin(); it != json.MemberEnd(); ++it)
		{
			if (!type.empty())
			{
				if (type == "popUpLayer")
				{
					initPopUpLayer(nullptr, static_cast<PopUpLayer*>(aParent), it);
				}
			}

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

void ViewBuilder::loadChildren(cocos2d::Node* aParent, rapidjson::Value& aChildrenValue)
{
	const cocos2d::Size& contentSize = aParent->getContentSize();

	for (auto it = aChildrenValue.MemberBegin(); it != aChildrenValue.MemberEnd(); ++it)
	{
		const std::string childType = it->name.GetString();
		if (childType == "sprite")
		{
			rapidjson::Value& attributes = it->value;
			cocos2d::Sprite* sprite = cocos2d::Sprite::create();
			if (sprite)
			{
				for (auto attrIt = attributes.MemberBegin(); attrIt != attributes.MemberEnd(); ++attrIt)
				{
					if (initNode(aParent, sprite, attrIt));
					else if (initSprite(aParent, sprite, attrIt));
				}
				aParent->addChild(sprite);
			}
		}
		else if (childType == "BMButton")
		{
			rapidjson::Value& attributes = it->value;
			BMButton* btn = createBMButton(attributes);
			
			if (btn)
			{
				for (auto attrIt = attributes.MemberBegin(); attrIt != attributes.MemberEnd(); ++attrIt)
				{
					initNode(aParent, btn, attrIt);
				}
				aParent->addChild(btn);
			}
		}
		else if (childType == "button")
		{
			rapidjson::Value& attributes = it->value;
			cocos2d::ui::Button* btn = createButton(attributes);
			if (btn)
			{
				for (auto attrIt = attributes.MemberBegin(); attrIt != attributes.MemberEnd(); ++attrIt)
				{
					initNode(aParent, btn, attrIt);
				}
				aParent->addChild(btn);
			}
		}
		else if (childType == "label")
		{
			rapidjson::Value& attributes = it->value;
			cocos2d::Label* label = createLabel(attributes);
			if (label != nullptr)
			{
				for (auto attrIt = attributes.MemberBegin(); attrIt != attributes.MemberEnd(); ++attrIt)
				{
					initNode(aParent, label , attrIt);
				}
				aParent->addChild(label);
			}
		}
	}
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
	else if (attrName == "children")
	{
		loadChildren(aObject, aAttrIt->value);
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

bool ViewBuilder::initPopUpLayer(const cocos2d::Node* aParent, PopUpLayer* aObject, rapidjson::Value::MemberIterator aAttrIt)
{
	const std::string attrName = aAttrIt->name.GetString();

	if (attrName == "appearingTime")
	{
		aObject->setAppeatingTime(static_cast<float>(aAttrIt->value.GetDouble()));
	}

	return true;
}

cocos2d::ui::Button* ViewBuilder::createButton(rapidjson::Value& aAttr)
{
	const sData& data = DM->getData();

	std::string normalFrameName;
	if (aAttr.HasMember("normal_image_frame_name"))
	{
		const auto imageIt = data.images.find(aAttr["normal_image_frame_name"].GetString());
		if (imageIt != data.images.end())
		{
			normalFrameName = imageIt->second;
		}
	}

	std::string selectedFrameName;
	if (aAttr.HasMember("selected_image_frame_name"))
	{
		const auto imageIt = data.images.find(aAttr["selected_image_frame_name"].GetString());
		if (imageIt != data.images.end())
		{
			selectedFrameName = imageIt->second;
		}
	}
	std::string disabledFrameName;
	if (aAttr.HasMember("disabled_image_frame_name"))
	{
		const auto imageIt = data.images.find(aAttr["disabled_image_frame_name"].GetString());
		if (imageIt != data.images.end())
		{
			disabledFrameName = imageIt->second;
		}
	}

	return cocos2d::ui::Button::create(normalFrameName,
		selectedFrameName,
		disabledFrameName,
		cocos2d::ui::Widget::TextureResType::PLIST);

}

BMButton* ViewBuilder::createBMButton(rapidjson::Value& aAttr)
{
	std::string normalFrameNameID;
	if (aAttr.HasMember("normal_image_frame_name"))
	{
		normalFrameNameID = aAttr["normal_image_frame_name"].GetString();
	}
	std::string selectedFrameNameID;
	if (aAttr.HasMember("selected_image_frame_name"))
	{
		selectedFrameNameID = aAttr["selected_image_frame_name"].GetString();
	}
	std::string disabledFrameNameID;
	if (aAttr.HasMember("disabled_image_frame_name"))
	{
		disabledFrameNameID = aAttr["disabled_image_frame_name"].GetString();
	}

	BMButton* btn = BMButton::create(normalFrameNameID, selectedFrameNameID, disabledFrameNameID);

	if (btn && aAttr.HasMember("font") && aAttr.HasMember("title"))
	{
		const std::string fontID = aAttr["font"].GetString();
		const std::string titleID = aAttr["title"].GetString();
		btn->setTitle(titleID, fontID);
	}

	return btn;
}

cocos2d::Label* ViewBuilder::createLabel(rapidjson::Value& aAttr)
{
	cocos2d::Label* label = nullptr;

	if (aAttr.HasMember("font") && aAttr.HasMember("text"))
	{
		const sData& data = DM->getData();

		std::string fontID = aAttr["font"].GetString();
		const auto fontIt = data.fonts.find(fontID);

		std::string stringID = aAttr["text"].GetString();
		const auto stringIt = data.strings.find(stringID);

		if ((fontIt != data.fonts.end()) && (stringIt != data.strings.end()))
		{
			label = cocos2d::Label::createWithBMFont(fontIt->second, stringIt->second);
			if (label != nullptr)
			{
				label->setColor(cocos2d::Color3B::BLACK);
			}
		}
	}

	return label;
}