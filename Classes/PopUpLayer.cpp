#include "PopUpLayer.h"

PopUpLayer::PopUpLayer()
	: mAppearingTime(0.3f)
	, mIsActive(false)
{
}

PopUpLayer::~PopUpLayer()
{
}

void PopUpLayer::show()
{
	if (!mIsActive)
	{
		activate();
		runAction(
			cocos2d::Spawn::createWithTwoActions(
				cocos2d::ScaleTo::create(mAppearingTime, 1.0f),
				cocos2d::FadeIn::create(mAppearingTime)
			)
		);
	}
}

void PopUpLayer::hide()
{
	if (mIsActive)
	{
		runAction(
			cocos2d::Sequence::create(
				cocos2d::Spawn::createWithTwoActions(
					cocos2d::ScaleTo::create(mAppearingTime, 0.0f),
					cocos2d::FadeOut::create(mAppearingTime)
				),
				cocos2d::CallFunc::create(CC_CALLBACK_0(PopUpLayer::deactivate, this)),
				nullptr
			)
		);
	}
}

bool PopUpLayer::init()
{
	bool result = false;
	if (Parent::init())
	{
		//setContentSize(cocos2d::Director::getInstance()->getVisibleSize());
		//setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
		setScale(0.0f);
		setOpacity(0.0f);
		setCascadeOpacityEnabled(true);
		//_eventDispatcher->pauseEventListenersForTarget(this);
		//this->ignoreAnchorPointForPosition(false);
		setVisible(false);
		result = true;
	}
	return result;
}

void PopUpLayer::deactivate()
{
	setVisible(false);
	mIsActive = false;
}

void PopUpLayer::activate()
{
	setVisible(true);
	mIsActive = true;
}

bool PopUpLayer::isActive()
{
	return mIsActive;
}

void PopUpLayer::setAppeatingTime(const float aAppearingTime)
{
	mAppearingTime = aAppearingTime;
}