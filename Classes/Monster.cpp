#include "Monster.h"
#include "DataManager.h"

Monster::Monster(const sMonster& aMonsterData)
	: mData(aMonsterData)
	, mCurrentHP(0.0f)
	, mDeathListener(nullptr)
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
		}
	}

	const float apearingTime = 0.4f;
	setOpacity(0);
	runAction(cocos2d::FadeIn::create(apearingTime));

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

void Monster::onTouched()
{
	const float actionTime = 0.15f;
	cocos2d::TintTo* action = cocos2d::TintTo::create(actionTime, cocos2d::Color3B::RED);
	cocos2d::TintTo* actionReverse = cocos2d::TintTo::create(actionTime, cocos2d::Color3B::WHITE);
	runAction(cocos2d::Sequence::create(action, actionReverse, nullptr));

	--mCurrentHP;

	if (mCurrentHP <= 0)
	{
		const float actionTime = 1.0f;
		cocos2d::RotateBy* rotation = cocos2d::RotateBy::create(actionTime, 360);
		cocos2d::ScaleTo* scaling = cocos2d::ScaleTo::create(actionTime, 0.1f);
		cocos2d::FadeOut* disapearing = cocos2d::FadeOut::create(actionTime);
		cocos2d::MoveBy* moving = cocos2d::MoveBy::create(1.0f, cocos2d::Vec2(0, 200));
		runAction(cocos2d::Sequence::create(
			cocos2d::Spawn::create(
				rotation,
				scaling,
				disapearing,
				moving,
				nullptr
			),
			cocos2d::CallFunc::create([this]() { removeFromParent(); }),
			nullptr
		));
		if (mDeathListener != nullptr)
		{
			mDeathListener->onMonsterDied();
		}
	}
}

void Monster::setDeathListener(MonsterDeathListener* aListener)
{
	mDeathListener = aListener;
}
