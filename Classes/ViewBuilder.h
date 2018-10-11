#ifndef VIEWBUILDER_H
#define VIEWBUILDER_H

#include "cocos2d.h"
#include "json/document.h"

class ViewBuilder
{
private:
	static void loadChildren(cocos2d::Node* aParent, rapidjson::Value& aChildrenValue);
	static bool initNode(const cocos2d::Node* aParent, cocos2d::Node* aObject, rapidjson::Value::MemberIterator aAttrIt);
	static bool initSprite(const cocos2d::Node* aParent, cocos2d::Sprite* aObject, rapidjson::Value::MemberIterator aAttrIt);
public:
	static bool loadFromJson(cocos2d::Node* aParent, const std::string& aJson);
};

#endif //VIEWBUILDER_H
