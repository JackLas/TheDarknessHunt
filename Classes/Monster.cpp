#include "Monster.h"
#include "DataManager.h"
#include "MonsterActionListener.h"

Monster::Monster(const sMonster& aMonsterData)
	: mData(aMonsterData)
	, mCurrentHP(0.0f)
	, mActionListener(nullptr)
	, mHealingTimer(0.0f)
	, mElapsedHealingTime(0.0f)
{
}

bool Monster::init()
{
	bool result = false;
	const auto& imageIt = DM->getData().images.find(mData.spriteFrameNameID);
	if (imageIt != DM->getData().images.end())
	{
		if (Parent::initWithSpriteFrameName(imageIt->second))
		{
			result = true;
			mCurrentHP = mData.hp;

			scheduleUpdate();
			setOpacity(0);
			runAction(cocos2d::FadeIn::create(mData.appearingTime));	
		}
	}

	return result;
}

Monster* Monster::create(const sMonster& aMonsterData)
{
	Monster* pRet = new (std::nothrow) Monster(aMonsterData);
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

Monster::~Monster()
{
}

void Monster::onEnter()
{
	Parent::onEnter();
	if (mActionListener != nullptr)
	{
		mActionListener->onMonsterSpawned(this);
	}
	mHealingTimer = mData.healingTime;
}

void Monster::setActionListener(MonsterActionListener* aListener)
{
	mActionListener = aListener;
}

void Monster::update(float aDeltaTime)
{
	mElapsedHealingTime += aDeltaTime;
	if (mElapsedHealingTime >= 0.1f)
	{
		mHealingTimer -= 0.1f;
		mElapsedHealingTime = 0.0f;

		if (mHealingTimer <= 0.0f)
		{
			mCurrentHP = mData.hp;
			mHealingTimer = mData.healingTime;

			const float actionTime = mData.appearingTime / 2.0f;
			cocos2d::TintTo* action = cocos2d::TintTo::create(actionTime, cocos2d::Color3B::GREEN);
			cocos2d::TintTo* actionReverse = cocos2d::TintTo::create(actionTime, cocos2d::Color3B::WHITE);
			runAction(cocos2d::Sequence::create(action, actionReverse, nullptr));

			if (mActionListener != nullptr)
			{
				mActionListener->onHealthRestored(this);
			}
		}
		else
		{
			if (mActionListener != nullptr)
			{
				mActionListener->onHealingTimerUpdated(this, mHealingTimer);
			}
		}
	}
}

const sMonster& Monster::getData() const
{
	return mData;
}

const std::string& Monster::getName() const 
{
	const std::map<std::string, std::string>& strings = DM->getData().strings;
	const auto& nameIt = strings.find(mData.nameSTID);
	if (nameIt != strings.end())
	{
		return nameIt->second;
	}
	return mData.nameSTID;
}

float Monster::getCurrentHealth() const
{
	return mCurrentHP;
}


float Monster::getCurrentHealthInPercent() const
{
	return mCurrentHP / mData.hp * 100;
}

const sResistance& Monster::getResistance() const
{
	return mData.resistance;
}

void Monster::onTouched()
{
	const float& actionTime = mData.touchActionTime;
	cocos2d::TintTo* action = cocos2d::TintTo::create(actionTime, cocos2d::Color3B::RED);
	cocos2d::TintTo* actionReverse = cocos2d::TintTo::create(actionTime, cocos2d::Color3B::WHITE);
	runAction(cocos2d::Sequence::create(action, actionReverse, nullptr));

	--mCurrentHP;

	if (mCurrentHP <= 0)
	{
		unscheduleUpdate();
		if (mActionListener != nullptr)
		{
			mActionListener->onMonsterDied(this);
		}
	}
}

void Monster::startDeathAnimation(cocos2d::FiniteTimeAction* aAnimation, bool aDirection)
{
	//setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	const float& disappearingTime = mData.disappearingTime;
	float rotationAngle = 720.0f;
	rotationAngle *= aDirection ? 1 : -1;
	cocos2d::RotateBy* rotation = cocos2d::RotateBy::create(disappearingTime, rotationAngle);
	cocos2d::ScaleTo* scaling = cocos2d::ScaleTo::create(disappearingTime, 0.1f);
	cocos2d::FadeOut* disapearing = cocos2d::FadeOut::create(disappearingTime);
	runAction(cocos2d::Sequence::create(
		cocos2d::Spawn::create(
			rotation,
			scaling,
			disapearing,
			aAnimation,
			nullptr
		),
		cocos2d::CallFunc::create([this]() { removeFromParentAndCleanup(true); }),
		nullptr
	));
}
