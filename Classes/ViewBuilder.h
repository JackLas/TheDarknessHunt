#ifndef VIEWBUILDER_H
#define VIEWBUILDER_H

#include "cocos2d.h"
#include "ui/UIButton.h"
#include "ui/CocosGUI.h"

#include "json/document.h"

#include "PopUpLayer.h"

class ViewBuilder
{
private:
	static cocos2d::Node* initChild(cocos2d::Node* aParent, rapidjson::Value::MemberIterator aChildIt);
	static void loadChildren(cocos2d::Node* aParent, rapidjson::Value& aChildrenValue);
	static void loadScrollViewParts(cocos2d::ui::ScrollView* aParent, rapidjson::Value& aChildrenValue);

	static bool initNode(const cocos2d::Node* aParent, cocos2d::Node* aObject, rapidjson::Value::MemberIterator aAttrIt);
	static bool initSprite(const cocos2d::Node* aParent, cocos2d::Sprite* aObject, rapidjson::Value::MemberIterator aAttrIt);
	static bool initPopUpLayer(const cocos2d::Node* aParent, PopUpLayer* aObject, rapidjson::Value::MemberIterator aAttrIt);
	static bool initButton(const cocos2d::Node* aParent, cocos2d::ui::Button* aObject, rapidjson::Value::MemberIterator aAttrIt);
	static bool initLabel(const cocos2d::Node* aParent, cocos2d::Label* aObject, rapidjson::Value::MemberIterator aAttrIt);
	static bool initScrollView(const cocos2d::Node* aParent, cocos2d::ui::ScrollView* aObject, rapidjson::Value::MemberIterator aAttrIt);

	static cocos2d::ui::Button* createButton(const rapidjson::Value& aAttr);
	static cocos2d::Label* createLabel(const rapidjson::Value& aAttr);

	static cocos2d::Color3B getColorFromValue(const rapidjson::Value& aAttr);

public:
	static bool loadFromJson(cocos2d::Node* aParent, const std::string& aJson);
};

#endif //VIEWBUILDER_H
