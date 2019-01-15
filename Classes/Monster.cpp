#include "Monster.h"
#include "DataManager.h"

Monster::Monster(const sMonster& aMonsterData)
	: mData(aMonsterData)
	, mCurrentHealth(0.0f)
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
			mCurrentHealth = mData.hp;
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
	return mCurrentHealth;
}

void Monster::onTouched()
{
	const float actionTime = 0.15f;
	cocos2d::TintTo* action = cocos2d::TintTo::create(actionTime, cocos2d::Color3B::RED);
	cocos2d::TintTo* actionReverse = cocos2d::TintTo::create(actionTime, cocos2d::Color3B::WHITE);
	runAction(cocos2d::Sequence::create(action, actionReverse, nullptr));
}
