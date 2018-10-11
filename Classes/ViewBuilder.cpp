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
		for (auto it = json.MemberBegin(); it != json.MemberEnd(); ++it)
		{
			const std::string itName = it->name.GetString();
			if (itName == "children")
			{
				loadChildren(aParent, it->value);
			}
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
			BMButton* btn = createBMButton(it->value);
			rapidjson::Value& attributes = it->value;
			if (btn)
			{
				for (auto attrIt = attributes.MemberBegin(); attrIt != attributes.MemberEnd(); ++attrIt)
				{
					initNode(aParent, btn, attrIt);
				}
				aParent->addChild(btn);
			}
		}
	}
}

bool ViewBuilder::initNode(const cocos2d::Node* aParent, cocos2d::Node* aObject, rapidjson::Value::MemberIterator aAttrIt)
{
	const cocos2d::Size& contentSize = aParent->getContentSize();
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