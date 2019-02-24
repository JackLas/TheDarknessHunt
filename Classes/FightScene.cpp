#include "FightScene.h"
#include "DataManager.h"
#include "MapScene.h"
#include "Player.h"
#include <iomanip>

FightScene::FightScene(const std::string& aLevelID)
	: mLevelID(aLevelID)
	, mSpawner(mLevelID)
	, mCurrentMonster(nullptr)
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
    } 
    else 
    { 
        delete pRet; 
        pRet = nullptr; 
    }
	return pRet;
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
				const sDamage& passiveDamage = PLAYER->getPassiveDamage();
				cocos2d::Node* labelParent = bar->getChildByName("passive_phys_damage");
				if (labelParent != nullptr)
				{
					cocos2d::Label* passivePhysDmgLabel = labelParent->getChildByName<cocos2d::Label*>("text");
					if (passivePhysDmgLabel != nullptr)
					{
						passivePhysDmgLabel->setString(std::to_string(passiveDamage.physical));
					}
				}
				labelParent = bar->getChildByName("passive_mag_damage");
				if (labelParent != nullptr)
				{
					cocos2d::Label* passiveMagDmgLabel = labelParent->getChildByName<cocos2d::Label*>("text");
					if (passiveMagDmgLabel != nullptr)
					{
						passiveMagDmgLabel->setString(std::to_string(passiveDamage.magical));
					}
				}

				const sDamage& clickDamage = PLAYER->getClickDamage();
				labelParent = bar->getChildByName("click_phys_damage");
				if (labelParent != nullptr)
				{
					cocos2d::Label* clickPhysDmgLabel = labelParent->getChildByName<cocos2d::Label*>("text");
					if (clickPhysDmgLabel != nullptr)
					{
						clickPhysDmgLabel->setString(std::to_string(clickDamage.physical));
					}
				}
				labelParent = bar->getChildByName("click_mag_damage");
				if (labelParent != nullptr)
				{
					cocos2d::Label* clickMagDmgLabel = labelParent->getChildByName<cocos2d::Label*>("text");
					if (clickMagDmgLabel != nullptr)
					{
						clickMagDmgLabel->setString(std::to_string(clickDamage.magical));
					}
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
					
					cocos2d::Vec2 barPosition;
					barPosition.x = bar->getBoundingBox().getMinX();
					barPosition.y = bar->getBoundingBox().getMinY();
					mGoldIconPosition = barPosition + labelParent->getPosition();
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

			bar = getChildByName("resist_bar");
			if (bar != nullptr)
			{
				mPhysResistLabel = bar->getChildByName<cocos2d::Label*>("phys_resist_label");
				mMagResistLabel = bar->getChildByName<cocos2d::Label*>("mag_resist_label");
			}

			updateKillsLabel();
			updateGoldLabel();
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
		const float initialPositionY = topBar->getPosition().y;
		const float shiftSize = topBar->getBoundingBox().size.height;
		topBar->setPositionY(initialPositionY + shiftSize);
		topBar->runAction(cocos2d::MoveBy::create(appearingTime, cocos2d::Vec2(0.0f, -shiftSize)));

		cocos2d::Node* resistBar = getChildByName("resist_bar");
		if (resistBar != nullptr)
		{
			const cocos2d::Vec2 pos = resistBar->getPosition();
			resistBar->setPositionY(initialPositionY);
			resistBar->runAction(cocos2d::MoveTo::create(appearingTime, pos));
		}
	}
	cocos2d::Node* bottomBar = getChildByName("bottom_bar");
	if (bottomBar != nullptr)
	{
		const float initialPositionY = bottomBar->getPosition().y;
		const float shiftSize = bottomBar->getBoundingBox().size.height;
		bottomBar->setPositionY(initialPositionY - shiftSize);
		bottomBar->runAction(cocos2d::MoveBy::create(appearingTime, cocos2d::Vec2(0.0f, +shiftSize)));
	}
	cocos2d::Node* btnMap = getChildByName("button_map");
	if (btnMap != nullptr)
	{
		btnMap->setOpacity(0.0f);
		btnMap->runAction(cocos2d::FadeIn::create(appearingTime));
	}

	respawnMonster();
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
			mCurrentMonster->takeDamage(PLAYER->getClickDamage());
			if (mMonsterHealthBar != nullptr)
			{
				const float percent = mCurrentMonster->getCurrentHealthInPercent();
				mMonsterHealthBar->setPercent(percent);
			}
		}
	}
}

void FightScene::givePlayerReward(const Monster* aMonster)
{
	const auto& images = DM->getData().images;
	const auto& frameNameIt = images.find("ICON_COIN");
	if (frameNameIt != images.end())
	{
		const float actionTime = 1.0f;
		cocos2d::Vec2 coinSpawnPosition;
		coinSpawnPosition.x = aMonster->getBoundingBox().getMidX();
		coinSpawnPosition.y = aMonster->getBoundingBox().getMidY();

		cocos2d::Size sceneSize = getBoundingBox().size;
		const float minJumpDestinationX = sceneSize.width * 0.1f;
		const float maxJumpDestinationX = sceneSize.width - (sceneSize.width * 0.1f);

		cocos2d::Node* bottomBar = getChildByName("bottom_bar");
		const float minJumpDestinationY = bottomBar->getBoundingBox().getMaxY();
		const float maxJumpDestinationY = minJumpDestinationY + sceneSize.height * 0.05f;

		const float jumpHeight = sceneSize.height / 3.0f;

		const int numOfAnimationCoins = 4;
		unsigned int rewardByCoin = aMonster->getGoldReward() / numOfAnimationCoins;
		for (int i = 0; i < numOfAnimationCoins; ++i)
		{
			cocos2d::Sprite* coin = cocos2d::Sprite::createWithSpriteFrameName(frameNameIt->second);
			cocos2d::Vec2 jumpDestination;
			jumpDestination.x = cocos2d::RandomHelper::random_real<float>(minJumpDestinationX, maxJumpDestinationX);
			jumpDestination.y = cocos2d::RandomHelper::random_real<float>(minJumpDestinationY, maxJumpDestinationY);
			coin->setPosition(coinSpawnPosition);
			coin->setLocalZOrder(11);
			coin->setOpacity(0.0f);

			coin->runAction(cocos2d::Sequence::create(
				cocos2d::DelayTime::create(cocos2d::RandomHelper::random_real<float>(0.0f, 0.5f)),
				cocos2d::Spawn::create(
					cocos2d::FadeIn::create(actionTime / 1.5f),
					cocos2d::JumpTo::create(actionTime, jumpDestination, jumpHeight, 1),
					nullptr
				),
				cocos2d::DelayTime::create(cocos2d::RandomHelper::random_real<float>(1.0f, 1.5f)),
				cocos2d::MoveTo::create(0.5f, mGoldIconPosition),
				cocos2d::CallFunc::create([this, coin, rewardByCoin]() { 
					coin->removeFromParent();
					PLAYER->addGold(rewardByCoin);
					updateGoldLabel();
				}),
				nullptr
			));

			addChild(coin);
		}
	}
}

void FightScene::updateGoldLabel()
{
	const unsigned int& gold = PLAYER->getGold();
	mGoldLabel->setString(std::to_string(gold));
}

void FightScene::updateKillsLabel()
{
	const unsigned int& kills = PLAYER->getKills(mLevelID);
	mKillsLabel->setString(std::to_string(kills));
}

void FightScene::respawnMonster()
{
	mCurrentMonster = mSpawner.getNextMonster();
	if (mCurrentMonster != nullptr)
	{
		mCurrentMonster->setActionListener(this);
		addChild(mCurrentMonster);
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
	
	float height = getBoundingBox().size.height / 3.0f;
	const float& animationTime = aMonster->getData().disappearingTime;
	cocos2d::JumpTo* action = cocos2d::JumpTo::create(animationTime, destination, height, 1);
	aMonster->startDeathAnimation(action, mIsMonsterDeathAnimationDirectionRight);

	PLAYER->addKill(mLevelID);
	updateKillsLabel();

	givePlayerReward(aMonster);

	respawnMonster();
}

void FightScene::onMonsterSpawned(const Monster* aMonster)
{
	if (mMonsterNameLabel != nullptr)
	{
		const std::string& spawnedMonsterName = aMonster->getName();
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
	if (mMonsterHealthBar != nullptr)
	{
		const float percent = aMonster->getCurrentHealthInPercent();
		mMonsterHealthBar->setPercent(percent);
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
