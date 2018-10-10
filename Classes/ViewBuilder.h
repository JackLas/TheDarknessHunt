#ifndef VIEWBUILDER_H
#define VIEWBUILDER_H

#include "cocos2d.h"
#include "json/document.h"

class ViewBuilder
{
private:
	static void loadFromValue(cocos2d::Node* aParent, rapidjson::Value& aValue);
public:
	static bool loadFromJson(cocos2d::Node* aParent, const std::string& aJson);
};

#endif //VIEWBUILDER_H
