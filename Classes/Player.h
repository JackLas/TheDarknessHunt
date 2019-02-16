#ifndef PLAYER_H
#define PLAYER_H

#include "cocos2d.h"
#include <ctime>
#include "dataTypes.h"

enum class eSlotID
{
	UNKNOWN_SLOT = 0,
	LEFT_SLOT,
	RIGHT_SLOT
};

struct sTeammate
{
	std::map<eSlotID, std::string> itemSlot;
	sDamage damage;

	sTeammate()
	{
		itemSlot[eSlotID::LEFT_SLOT];
		itemSlot[eSlotID::RIGHT_SLOT];
	}
};

class Player
{
private:
	std::string mTimeFile;
	std::string mSaveFile;
	std::string mHashFile;

	bool mIsNeedSaving;

	std::time_t mTimeBetweenGameLaunchings;
	cocos2d::Vec2 mMapPosition;
	std::map<std::string, unsigned int> mKills; 
	unsigned int mGold;
	std::map<eSlotID, std::string> mEquipedItems;
	sDamage mClickDamage;
	sDamage mPassiveDamage;
	std::vector<sTeammate> mTeam;
	std::vector<std::string> mInventory;

	Player();
	void loadData();

public:
	static Player* getInstance();
	~Player();

	void saveData();
	void resetProgress();

	const std::time_t& getTimeBetweenGameLaunchings() const;
	void setMapPosition(const cocos2d::Vec2& aPosition);
	const cocos2d::Vec2& getMapPosition() const;
	void addKill(const std::string& aLevelID, const unsigned int aAmount = 1);
	unsigned int getKills(const std::string& aLevelID) const;
	void addGold(const unsigned int aAmount);
	unsigned int getGold() const;
	const std::vector<sTeammate>& getTeam() const;
	const std::vector<std::string>& getInventory() const;
	bool equipItem(int aTeammateIndex, eSlotID aSlot, int aItemIndex);
	bool clearSlot(int aTeammateIndex, eSlotID aSlot);
	bool equipItem(eSlotID aSlot, int aItemIndex);
	bool clearSlot(eSlotID aSlot);
	bool sellItem(int aItemIndex);
	const std::string& getEquipedItem(eSlotID aSlotID);
	const std::string& getEquipedItem(int aTeammateIndex, eSlotID aSlotID);
	const sDamage& getClickDamage() const;
	const sDamage& getTeammateDamage(int aTeammateIndex) const;
	const sDamage& getPassiveDamage() const;
	int getCurrentHirePriceMultiplier() const;
};

#define PLAYER Player::getInstance()

#endif //PLAYER_H
