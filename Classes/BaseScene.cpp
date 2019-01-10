#include "BaseScene.h"
#include "ViewBuilder.h"
#include "viewObjectType.h"

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
		result &= ViewBuilder::loadFromJson(this, aView);
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
