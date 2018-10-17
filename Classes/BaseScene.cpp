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
	cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([]() {
		cocos2d::Director::getInstance()->getTextureCache()->removeUnusedTextures();
		cocos2d::SpriteFrameCache* cache = cocos2d::SpriteFrameCache::getInstance();
		cache->removeUnusedSpriteFrames();
	});
}

bool BaseScene::init(const std::string& aView)
{
	bool result = false;

	if (Parent::init())
	{
		result = true;
		loadSpriteCache();
		result &= ViewBuilder::loadFromJson(this, aView);
	}

	return result;
}

void BaseScene::initButtons(cocos2d::Vector<cocos2d::Node*>& aChildren,
	const std::function<void(cocos2d::Ref* aSender, cocos2d::ui::Widget::TouchEventType aEvent)>& aCallback)
{
	for (auto& child : aChildren)
	{
		if (static_cast<eViewObjectType>(child->getTag()) == eViewObjectType::OBEJCT_TYPE_BUTTON)
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
