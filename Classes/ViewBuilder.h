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
	class ViewComponent
	{	
	protected:
		const std::map<std::string, ViewComponent*>& mComponents;
		cocos2d::Color3B getColorFromValue(const rapidjson::Value& aAttr);
	public:
		ViewComponent(const std::map<std::string, ViewComponent*>& aComponents);
		virtual cocos2d::Node* create(const rapidjson::Value& aAttr) = 0;
		virtual bool init(  const cocos2d::Node* aParent, 
							cocos2d::Node* aObject, 
							rapidjson::Value::MemberIterator aAttrIt) = 0;
		virtual void addToParent(cocos2d::Node* aParent, cocos2d::Node* aChild) = 0;

	};

	class NodeComponent: public ViewComponent
	{
	public:
		NodeComponent(const std::map<std::string, ViewComponent*>& aComponents);
		virtual cocos2d::Node* create(const rapidjson::Value& aAttr) override;
		virtual bool init(	const cocos2d::Node* aParent, 
							cocos2d::Node* aObject, 
							rapidjson::Value::MemberIterator aAttrIt) override;
		virtual void addToParent(cocos2d::Node* aParent, cocos2d::Node* aChild) override;
	protected:
		cocos2d::Node* initChild(cocos2d::Node* aParent, rapidjson::Value::MemberIterator aChildIt);
		void loadChildren(cocos2d::Node* aParent, rapidjson::Value& aChildIt);
	};

	class SpriteComponent: public NodeComponent
	{
	public:
		SpriteComponent(const std::map<std::string, ViewComponent*>& aComponents);
		virtual cocos2d::Node* create(const rapidjson::Value& aAttr) override;
		virtual bool init(  const cocos2d::Node* aParent,
							cocos2d::Node* aObject,
							rapidjson::Value::MemberIterator aAttrIt) override;
	};

	class LabelComponent: public NodeComponent
	{
	public:
		LabelComponent(const std::map<std::string, ViewComponent*>& aComponents);
		virtual cocos2d::Node* create(const rapidjson::Value& aAttr) override;
		virtual bool init(  const cocos2d::Node* aParent,
							cocos2d::Node* aObject,
							rapidjson::Value::MemberIterator aAttrIt) override;
	};

	class ButtonComponent: public NodeComponent
	{
	public:
		ButtonComponent(const std::map<std::string, ViewComponent*>& aComponents);
		virtual cocos2d::Node* create(const rapidjson::Value& aAttr) override;
		virtual bool init(  const cocos2d::Node* aParent,
							cocos2d::Node* aObject,
							rapidjson::Value::MemberIterator aAttrIt) override;
	};

	class PopUpLayerComponent: public NodeComponent
	{
	public:
		PopUpLayerComponent(const std::map<std::string, ViewComponent*>& aComponents);
		virtual cocos2d::Node* create(const rapidjson::Value& aAttr) override;
		virtual bool init(  const cocos2d::Node* aParent,
							cocos2d::Node* aObject,
							rapidjson::Value::MemberIterator aAttrIt) override;
	};

	class MapScrollViewComponent: public NodeComponent
	{
	public:
		MapScrollViewComponent(const std::map<std::string, ViewComponent*>& aComponents);
		virtual cocos2d::Node* create(const rapidjson::Value& aAttr) override;
		virtual bool init(  const cocos2d::Node* aParent,
							cocos2d::Node* aObject,
							rapidjson::Value::MemberIterator aAttrIt) override;
		void loadParts(cocos2d::ui::ScrollView* aParent, rapidjson::Value& aPartsValue);
	};

	std::map<std::string, ViewComponent*> mComponents;
public:
	ViewBuilder();
	~ViewBuilder();
	bool loadFromJson(cocos2d::Node* aParent, const std::string& aJson);
};

#endif //VIEWBUILDER_H