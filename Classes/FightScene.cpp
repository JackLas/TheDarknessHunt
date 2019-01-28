#include "FightScene.h"
#include "DataManager.h"
#include "MapScene.h"
#include <iomanip>

FightScene::FightScene(const std::string& aLevelID)
	: mLevelID(aLevelID)
	, mSpawner(mLevelID)
	, mCurrentMonster(nullptr)
	, mPhysDamageLabel(nullptr)
	, mMagDamageLabel(nullptr)
	, mPhysResistLabel(nullptr)
	, mMagResistLabel(nullptr)
	, mKillsLabel(nullptr)
	, mGoldLabel(nullptr)
	, mMonsterNameLabel(nullptr)
	, mTimeToHealLabel(nullptr)
	, mMonsterHealthBar(nullptr)
	, mIsMonsterDeathAnimationDirectionRight(false)
{
}

FightScene::~FightScene()
{
}

FightScene* FightScene::create(const std::string& aLevelID)
{ 
	FightScene *pRet = new (std::nothrow) FightScene(aLevelID);
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

cocos2d::Scene* FightScene::createScene(const std::string& aLevelID)
{
	cocos2d::Scene* scene = cocos2d::Scene::create();
	cocos2d::Layer* layer = FightScene::create(aLevelID);
	if (layer != nullptr)
	{
		scene->addChild(layer);
	}
	return scene;
}

bool FightScene::init()
{
	const std::string& view = DM->getViewById("FIGHT_SCENE");
	bool result = false;

	if (Parent::init(view))
	{
		result = true;

		const auto& levelIt = DM->getData().levels.find(mLevelID);
		if (levelIt != DM->getData().levels.end())
		{
			const auto& levelData = levelIt->second;

			cocos2d::Sprite* bg = getChildByName<cocos2d::Sprite*>("bg");
			if (bg != nullptr)
			{
				const std::string frameName = DM->getFrameNameById(levelData.background);
				bg->initWithSpriteFrameName(frameName);
			}

			setButtonTouchListener(CC_CALLBACK_2(FightScene::onButtonTouched, this));
			auto touchListener = cocos2d::EventListenerTouchOneByOne::create();
			touchListener->setSwallowTouches(true);
			touchListener->onTouchBegan = CC_CALLBACK_2(FightScene::onTouchBegan, this);
			touchListener->onTouchMoved = CC_CALLBACK_2(FightScene::onTouchMoved, this);
			touchListener->onTouchEnded = CC_CALLBACK_2(FightScene::onTouchEnded, this);
			_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

			cocos2d::Vec2 position = getContentSize();
			position.x *= levelData.spawnPoint.x;
			position.y *= levelData.spawnPoint.y;
			mSpawner.setSpawnPoint(position);

			cocos2d::Node* bar = getChildByName("top_bar");
			if (bar != nullptr)
			{
				cocos2d::Node* labelParent = bar->getChildByName("phys_damage");
				if (labelParent != nullptr)
				{
					mPhysDamageLabel = labelParent->getChildByName<cocos2d::Label*>("text");
				}
				labelParent = bar->getChildByName("mag_damage");
				if (labelParent != nullptr)
				{
					mMagDamageLabel = labelParent->getChildByName<cocos2d::Label*>("text");
				}
				labelParent = bar->getChildByName("phys_resist");
				if (labelParent != nullptr)
				{
					mPhysResistLabel = labelParent->getChildByName<cocos2d::Label*>("text");
				}
				labelParent = bar->getChildByName("mag_resist");
				if (labelParent != nullptr)
				{
					mMagResistLabel = labelParent->getChildByName<cocos2d::Label*>("text");
				}
				labelParent = bar->getChildByName("kills");
				if (labelParent != nullptr)
				{
					mKillsLabel = labelParent->getChildByName<cocos2d::Label*>("text");
				}
				labelParent = bar->getChildByName("gold");
				if (labelParent != nullptr)
				{
					mGoldLabel = labelParent->getChildByName<cocos2d::Label*>("text");
				} 
			}
			bar = getChildByName("bottom_bar");
			if (bar != nullptr)
			{
				mMonsterNameLabel = bar->getChildByName<cocos2d::Label*>("mob_name");
				mTimeToHealLabel = bar->getChildByName<cocos2d::Label*>("time_to_heal");
				cocos2d::Node* healthBar = bar->getChildByName("health_bar");
				if (healthBar != nullptr)
				{
					mMonsterHealthBar = healthBar->getChildByName<cocos2d::ui::LoadingBar*>("core");
				}
			}

			mPhysDamageLabel->setString("0.5");
			mMagDamageLabel->setString("0.5");
			mKillsLabel->setString("0");
			mGoldLabel->setString("0");
		}
	}

	return result;
}

void FightScene::onEnter()
{
	Parent::onEnter();

	const float appearingTime = 0.5f;
	cocos2d::Node* topBar = getChildByName("top_bar");
	if (topBar != nullptr)
	{
		float initialPositionY = topBar->getPosition().y;
		float shiftSize = topBar->getContentSize().height;
		topBar->setPositionY(initialPositionY + shiftSize);
		topBar->runAction(cocos2d::MoveBy::create(appearingTime, cocos2d::Vec2(0.0f, -shiftSize)));
	}
	cocos2d::Node* bottomBar = getChildByName("bottom_bar");
	if (bottomBar != nullptr)
	{
		float initialPositionY = bottomBar->getPosition().y;
		float shiftSize = bottomBar->getContentSize().height;
		bottomBar->setPositionY(initialPositionY - shiftSize);
		bottomBar->runAction(cocos2d::MoveBy::create(appearingTime, cocos2d::Vec2(0.0f, +shiftSize)));
	}
	cocos2d::Node* btnMap = getChildByName("button_map");
	if (btnMap != nullptr)
	{
		btnMap->setOpacity(0.0f);
		btnMap->runAction(cocos2d::FadeIn::create(appearingTime));
	}

	updateMonster();
}

void FightScene::onButtonTouched(cocos2d::Ref* aSender, cocos2d::ui::Widget::TouchEventType aEvent)
{
	if (aEvent == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		cocos2d::ui::Button* btn = static_cast<cocos2d::ui::Button*>(aSender);
		const std::string& btnName = btn->getName();

		if (btnName == "button_map")
		{
			cocos2d::Director::getInstance()->replaceScene(MapScene::createScene());
		}
	}
}

bool FightScene::onTouchBegan(cocos2d::Touch* aTouch, cocos2d::Event* aEvent)
{
	return true;
}

void FightScene::onTouchMoved(cocos2d::Touch* aTouch, cocos2d::Event* aEvent)
{
}

void FightScene::onTouchEnded(cocos2d::Touch* aTouch, cocos2d::Event* aEvent)
{
	const cocos2d::Vec2 touchPosition = convertTouchToNodeSpace(aTouch);
	if (mCurrentMonster != nullptr)
	{
		if (mCurrentMonster->getBoundingBox().containsPoint(touchPosition))
		{
			mCurrentMonster->onTouched();
			if (mMonsterHealthBar != nullptr)
			{
				mMonsterHealthBar->setPercent(mCurrentMonster->getCurrentHealthInPercent());
			}
		}
	}
}

void FightScene::updateMonster()
{
	mCurrentMonster = mSpawner.getNextMonster();
	if (mCurrentMonster != nullptr)
	{
		mCurrentMonster->setActionListener(this);
		addChild(mCurrentMonster);
		if (mMonsterHealthBar != nullptr)
		{
			mMonsterHealthBar->setPercent(mCurrentMonster->getCurrentHealthInPercent());
		}
	}
}

void FightScene::onMonsterDied(Monster* aMonster)
{
	mIsMonsterDeathAnimationDirectionRight = !mIsMonsterDeathAnimationDirectionRight;

	cocos2d::Vec2 destination = aMonster->getPosition();
	if (mIsMonsterDeathAnimationDirectionRight)
	{
		destination.x = getBoundingBox().size.width;
	}
	else
	{
		destination.x = 0;
	}
	
	float height = getBoundingBox().size.height / 2.0f;
	const float& animationTime = aMonster->getData().disappearingTime;
	cocos2d::JumpTo* action = cocos2d::JumpTo::create(animationTime, destination, height, 1);
	aMonster->startDeathAnimation(action, mIsMonsterDeathAnimationDirectionRight);

	updateMonster();
}

void FightScene::onMonsterSpawned(const Monster* aMonster)
{
	const std::string& spawnedMonsterName = aMonster->getName();
	if (mMonsterNameLabel != nullptr)
	{
		mMonsterNameLabel->setString(spawnedMonsterName);
	}
	if (mPhysResistLabel != nullptr)
	{
		std::string value = std::to_string(aMonster->getResistance().physical);
		value += "%";
		mPhysResistLabel->setString(value);
	}
	if (mMagResistLabel != nullptr)
	{
		std::string value = std::to_string(aMonster->getResistance().magical);
		value += "%";
		mMagResistLabel->setString(value);
	}
}

void FightScene::onHealthRestored(const Monster* aMonster)
{
	if (mMonsterHealthBar != nullptr)
	{
		mMonsterHealthBar->setPercent(100.0f);
	}
}

void FightScene::onHealingTimerUpdated(const Monster* aMonster, const float& aTimeLeft)
{
	if (mTimeToHealLabel != nullptr)
	{
		std::stringstream str;
		str << std::fixed << std::setprecision(1) << aTimeLeft;
		mTimeToHealLabel->setString(str.str());
	}
}
