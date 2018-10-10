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
				loadFromValue(aParent, it->value);
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

void ViewBuilder::loadFromValue(cocos2d::Node* aParent, rapidjson::Value& aValue)
{
	const cocos2d::Size& contentSize = aParent->getContentSize();

	for (auto it = aValue.MemberBegin(); it != aValue.MemberEnd(); ++it)
	{
		const std::string itName = it->name.GetString();
		if (itName == "sprite")
		{
			rapidjson::Value& attributes = it->value;
			cocos2d::Sprite* sprite = cocos2d::Sprite::create();
			for (auto atrIt = attributes.MemberBegin(); atrIt != attributes.MemberEnd(); ++atrIt)
			{
				const std::string atrName = atrIt->name.GetString();
				if (atrName == "frame_name")
				{
					const auto frameName = DM->getData().images.find(atrIt->value.GetString());
					if (frameName != DM->getData().images.end())
					{
						sprite->initWithSpriteFrameName(frameName->second);
					}
				}
				else if (atrName == "pos_x")
				{
					float posX = static_cast<float>(atrIt->value.GetDouble());
					sprite->setPositionX(contentSize.width * posX);
				}
				else if (atrName == "pos_y")
				{
					float posY = static_cast<float>(atrIt->value.GetDouble());
					sprite->setPositionY(contentSize.height * posY);
				}
			}
			

			aParent->addChild(sprite);
		}
	}
}