#ifndef CITYSCENE_H
#define CITYSCENE_H 

#include "cocos2d.h"
#include "BaseScene.h"

class CityScene : public BaseScene
{
private:
	typedef BaseScene Parent;

	cocos2d::Label* mGoldLabel;
	cocos2d::Label* mItemNameLabel;
	cocos2d::Label* mPhysDmgLabel;
	cocos2d::Label* mMagDmgLabel;
	cocos2d::Label* mPrice;
	cocos2d::Sprite* mItemPreview;

	std::string mSelectedItemID;

	CityScene();

protected:
	CREATE_FUNC(CityScene);
	virtual bool init();

	void updatePlayerGold();
	void initItemList();
	void onShopSlotTouched(cocos2d::Ref* aSender, cocos2d::ui::Widget::TouchEventType aEvent);
	void setSelectedFrame(cocos2d::Node* aItemBtn);
	void setSelectedItemInfo(const std::string& aSelectedItemID);
	void showMessage(const std::string& aMsg, const cocos2d::Color3B& aColor = cocos2d::Color3B::WHITE);
	void onButtonTouched(cocos2d::Ref* aSender, cocos2d::ui::Widget::TouchEventType aEvent);

public:
	static cocos2d::Scene* createScene();
	virtual ~CityScene();
};

#endif //CITYSCENE_H
