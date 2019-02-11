#include "BaseScene.h"
#include "viewObjectType.h"
#include "DataManager.h"

BaseScene::BaseScene()
{
}

BaseScene::~BaseScene()
{
}

void BaseScene::onEnter()
{
	Parent::onEnter();
}

bool BaseScene::init(const std::string& aView)
{
	bool result = false;

	if (Parent::init())
	{
		result = true;

		result &= DM->loadViewFromJson(this, aView);
	}

	return result;
}

void BaseScene::initButtons(cocos2d::Vector<cocos2d::Node*>& aChildren,
	const std::function<void(cocos2d::Ref* aSender, cocos2d::ui::Widget::TouchEventType aEvent)>& aCallback)
{
	for (auto& child : aChildren)
	{
		if (static_cast<eViewObjectType>(child->getTag()) == eViewObjectType::OBJECT_TYPE_BUTTON)
		{
			cocos2d::ui::Button* btn = static_cast<cocos2d::ui::Button*>(child);
			btn->addTouchEventListener(aCallback);
		}
		initButtons(child->getChildren(), aCallback);
	}
}

void BaseScene::setButtonTouchListener(const std::function<void(cocos2d::Ref* aSender, cocos2d::ui::Widget::TouchEventType aEvent)>& aCallback)
{
	initButtons(getChildren(), aCallback);
}

cocos2d::Node* BaseScene::findChildByName(const std::vector<std::string>& aSearchingSequence)
{
	cocos2d::Node* child = this;
	for (auto& childName : aSearchingSequence)
	{
		if (child != nullptr)
		{
			child = child->getChildByName(childName);
		}
		else
		{
			break;
		}
	}
	return (child == this) ? nullptr : child;
}
