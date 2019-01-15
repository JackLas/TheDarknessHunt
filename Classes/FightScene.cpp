#include "FightScene.h"
#include "DataManager.h"
#include "MapScene.h"

FightScene::FightScene(const sLevel& aLevelData)
	: mLevelData(aLevelData)
	, mSpawner(mLevelData.monsters)
	, mCurrentMonster(nullptr)
{
}

FightScene::~FightScene()
{
}

FightScene* FightScene::create(const sLevel& aLevelData)
{ 
	FightScene *pRet = new (std::nothrow) FightScene(aLevelData);
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

cocos2d::Scene* FightScene::createScene(const sLevel& aLevelData)
{
	cocos2d::Scene* scene = cocos2d::Scene::create();
	cocos2d::Layer* layer = FightScene::create(aLevelData);
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

		cocos2d::Sprite* bg = getChildByName<cocos2d::Sprite*>("bg");
		if (bg != nullptr)
		{
			const std::string frameName = DM->getFrameNameById(mLevelData.background);
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
		position.x *= mLevelData.spawnPoint.x;
		position.y *= mLevelData.spawnPoint.y;
		mSpawner.setSpawnPoint(position);

		updateMonster();
	}

	return result;
}

void FightScene::onEnter()
{
	Parent::onEnter();

	const float appearingTime = 0.5f;
	cocos2d::Sprite* topBar = getChildByName<cocos2d::Sprite*>("top_bar");
	if (topBar != nullptr)
	{
		float initialPositionY = topBar->getPosition().y;
		float shiftSize = topBar->getContentSize().height;
		topBar->setPositionY(initialPositionY + shiftSize);
		topBar->runAction(cocos2d::MoveBy::create(appearingTime, cocos2d::Vec2(0.0f, -shiftSize)));
	}
	cocos2d::Sprite* bottomBar = getChildByName<cocos2d::Sprite*>("bottom_bar");
	if (bottomBar != nullptr)
	{
		float initialPositionY = bottomBar->getPosition().y;
		float shiftSize = bottomBar->getContentSize().height;
		bottomBar->setPositionY(initialPositionY - shiftSize);
		bottomBar->runAction(cocos2d::MoveBy::create(appearingTime, cocos2d::Vec2(0.0f, +shiftSize)));
	}
}

void FightScene::onButtonTouched(cocos2d::Ref* aSender, cocos2d::ui::Widget::TouchEventType aEvent)
{
	if (aEvent == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		cocos2d::ui::Button* btn = static_cast<cocos2d::ui::Button*>(aSender);
		const std::string& btnName = btn->getName();

		if (btnName == "back")
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
		}
	}
}

void FightScene::updateMonster()
{
	mCurrentMonster = mSpawner.getNextMonster();
	if (mCurrentMonster != nullptr)
	{
		mCurrentMonster->setDeathListener(this);
		addChild(mCurrentMonster);
	}
}

void FightScene::onMonsterDied()
{
	updateMonster();
}