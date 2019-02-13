#ifndef CAMPSCENE_H
#define CAMPSCENE_H 

#include "cocos2d.h"
#include "BaseScene.h"
#include "Player.h"

class CampScene : public BaseScene
{
private:
	typedef BaseScene Parent;

	cocos2d::Label* mGoldLabel;

	cocos2d::Node* mCurrentVisibleLayer;
	const int mUnknownIndex;
	int mSelectedTeammateIndex;
	eSlotID mSelectedSlot;
	int mSelectedItemIndex;

	CampScene();
protected:
	CREATE_FUNC(CampScene);
	virtual bool init();

	void updateGoldLabel();
	void onButtonTouched(cocos2d::Ref* aSender, cocos2d::ui::Widget::TouchEventType aEvent);

	bool replaceVisibleLayer(const std::string& aLayerName);

	void initTeammateLayer();
	void initTeammateSlotContent(const sTeammate& aTeammate, cocos2d::Node* aSlot, eSlotID aSlotID);
	void onTeammateInventorySlotTouched(cocos2d::Ref* aSender, cocos2d::ui::Widget::TouchEventType aEvent);

	void initInventoryLayer();
	void clearInventoryLayer();
	void setInventoryEquipMode();
	void setInventoryViewMode();
	void initItemInfo(const std::string& aItemID);
	void onInventorySlotTouched(cocos2d::Ref* aSender, cocos2d::ui::Widget::TouchEventType aEvent);

	void initPlayerLayer();
	void initPlayerSlotContent(cocos2d::Node* aSlot, eSlotID aSlotID);
	void initPlayerDamageLabels();
	void onPlayerSlotTouched(cocos2d::Ref* aSender, cocos2d::ui::Widget::TouchEventType aEvent); 

public:
	static cocos2d::Scene* createScene();
	virtual ~CampScene();
};

#endif //CAMPSCENE_H
