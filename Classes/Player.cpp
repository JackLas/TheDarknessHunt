#include "Player.h"
#include <string>
#include <chrono>
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/prettywriter.h"
#include "DataManager.h"

Player::Player()
	: mIsNeedSaving(false)
	, mTimeBetweenGameLaunchings(0)
	, mGold(1000)
{
	mTimeFile = cocos2d::FileUtils::getInstance()->getWritablePath() + "/data.time";
	mSaveFile = cocos2d::FileUtils::getInstance()->getWritablePath() + "/data.save";
	mHashFile = cocos2d::FileUtils::getInstance()->getWritablePath() + "/data.hash";

	loadData();

	const float timeSavingInterval = 1.0f;
	auto scheduler = cocos2d::Director::getInstance()->getScheduler();
	scheduler->schedule([this](float aDt)
	{
		typedef std::chrono::system_clock clock;
		cocos2d::FileUtils::getInstance()->writeStringToFile(
			std::to_string(clock::to_time_t(clock::now())), mTimeFile);

	}, this, timeSavingInterval, CC_REPEAT_FOREVER, 0.0f, false, "timeSaver");

	const float dataSavingInterval = 5.0f;
	scheduler->schedule([this](float aDt)
	{
		if (mIsNeedSaving)
		{
			saveData();
			mIsNeedSaving = false;
		}
	}, this, dataSavingInterval, CC_REPEAT_FOREVER, 0.0f, false, "dataSaver");
	
	mEquipedItems[eSlotID::LEFT_SLOT];
	mEquipedItems[eSlotID::RIGHT_SLOT];

	//--- for tests ---

	while(false)
	//for (int i = 0; i <= 26; ++i)
	{
		int rand = cocos2d::RandomHelper::random_int<int>(0, 2);
		switch (rand)
		{
		case 0:
			mInventory.push_back("OLD_STAFF");
			break;
		case 1:
			mInventory.push_back("OLD_SWORD");
			break;
		default:
			mInventory.push_back("STRANGE_AMULET");
		}

	}
	//--- end of test ---
}

Player::~Player()
{
}

Player* Player::getInstance()
{
	static Player player;
	return &player;
}

void Player::loadData()
{
	typedef std::chrono::system_clock clock;
	if (cocos2d::FileUtils::getInstance()->isFileExist(mTimeFile))
	{
		std::string timeData = cocos2d::FileUtils::getInstance()->getStringFromFile(mTimeFile);
		mTimeBetweenGameLaunchings = clock::to_time_t(clock::now()) - std::stoll(timeData);
	}

	if (cocos2d::FileUtils::getInstance()->isFileExist(mSaveFile) &&
		cocos2d::FileUtils::getInstance()->isFileExist(mHashFile))
	{
		std::string	currentSaveHash = cocos2d::utils::getFileMD5Hash(mSaveFile);
		std::string realSaveHash = cocos2d::FileUtils::getInstance()->getStringFromFile(mHashFile);
		if (currentSaveHash == realSaveHash)
		{
			rapidjson::Document save;
			std::string configContent = cocos2d::FileUtils::getInstance()->getStringFromFile(mSaveFile);
			save.Parse(configContent.c_str());
			if (!save.HasParseError())
			{
				for (auto it = save.MemberBegin(); it != save.MemberEnd(); ++it)
				{
					std::string name = it->name.GetString();
					if (name == "kills")
					{
						rapidjson::Value& levels = it->value;
						for (auto level = levels.MemberBegin(); level != levels.MemberEnd(); ++level)
						{
							std::string levelName = level->name.GetString();
							mKills[levelName] = static_cast<unsigned int>(level->value.GetInt());
						}
					}
					else if (name == "gold")
					{
						mGold = static_cast<unsigned int>(it->value.GetInt());
					}
				}
			}
			else
			{
				CCLOG("save file parsing error");
			}
		}
	}
}

void Player::saveData()
{
	rapidjson::Document saveData;
	saveData.SetObject();
	rapidjson::Value killsObject;
	killsObject.SetObject();
	for (const auto& kills : mKills)
	{
		rapidjson::Value killsValue(kills.second);
		char buffer[64];
		int len = sprintf(buffer, "%s", kills.first.c_str());
		rapidjson::Value killsName;
		killsName.SetString(buffer, len, saveData.GetAllocator());
		killsObject.AddMember(killsName, killsValue, saveData.GetAllocator());
	}
	saveData.AddMember("kills", killsObject, saveData.GetAllocator());
	rapidjson::Value gold(mGold);
	saveData.AddMember("gold", gold, saveData.GetAllocator());

	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
	saveData.Accept(writer);
	cocos2d::FileUtils::getInstance()->writeStringToFile(buffer.GetString(), mSaveFile);

	std::string saveHash = cocos2d::utils::getFileMD5Hash(mSaveFile);
	cocos2d::FileUtils::getInstance()->writeStringToFile(saveHash, mHashFile);
}

void Player::resetProgress()
{
	mGold = 1000;
	mKills.clear();
	cocos2d::FileUtils::getInstance()->removeFile(mSaveFile);
	cocos2d::FileUtils::getInstance()->removeFile(mHashFile);
	mIsNeedSaving = false;
}

const std::time_t& Player::getTimeBetweenGameLaunchings() const 
{
	return mTimeBetweenGameLaunchings;
}

void Player::setMapPosition(const cocos2d::Vec2& aPosition)
{
	mMapPosition = aPosition;
}

const cocos2d::Vec2& Player::getMapPosition() const 
{
	return mMapPosition;
}

void Player::addKill(const std::string& aLevelID, const unsigned int aAmount)
{
	const auto levelIt = mKills.find(aLevelID);
	if (levelIt != mKills.end())
	{
		levelIt->second += aAmount;
	}
	else
	{
		mKills[aLevelID] = aAmount;
	}
	mIsNeedSaving = true;
}

unsigned int Player::getKills(const std::string& aLevelID) const
{
	const auto levelIt = mKills.find(aLevelID);
	if (levelIt != mKills.end())
	{
		return levelIt->second;
	}
	else
	{
		return 0;
	}
}

void Player::addGold(const unsigned int aAmount)
{
	mGold += aAmount;
	mIsNeedSaving = true;
}

unsigned int Player::getGold() const
{
	return mGold;
}

const std::vector<sTeammate>& Player::getTeam() const
{
	return mTeam;
}

const std::vector<std::string>& Player::getInventory() const
{
	return mInventory;
}

bool Player::equipItem(int aTeammateIndex, eSlotID aSlot, int aItemIndex)
{
	bool result = false;

	if ((aTeammateIndex > -1) &&
		(aTeammateIndex < mTeam.size()) &&
		(aSlot != eSlotID::UNKNOWN_SLOT) &&
		(aItemIndex > -1) &&
		(aItemIndex < mInventory.size()))
	{
		sTeammate& teammate = mTeam[aTeammateIndex];
		std::string& item = mInventory[aItemIndex];
		const auto& slotIt = teammate.itemSlot.find(aSlot);
		if (slotIt != teammate.itemSlot.end())
		{
			const auto& items = DM->getData().items;
			auto itemIt = items.find(item);
			if (itemIt != items.end())
			{
				std::string& equipedItem = slotIt->second;
				if (!equipedItem.empty())
				{
					auto equipedItemIt = items.find(equipedItem);
					if (equipedItemIt != items.end())
					{
						teammate.damage -= equipedItemIt->second.damage;
						mPassiveDamage -= equipedItemIt->second.damage;
						mInventory.push_back(equipedItem);
					}
				}
				teammate.damage += itemIt->second.damage;
				mPassiveDamage += itemIt->second.damage;
				equipedItem = item;
				mInventory.erase(mInventory.begin() + aItemIndex);
				result = true;
			}
		}
	}

	return result;
}

bool Player::clearSlot(int aTeammateIndex, eSlotID aSlot)
{
	bool result = false;

	if ((aTeammateIndex > -1) &&
		(aTeammateIndex < mTeam.size()) &&
		(aSlot != eSlotID::UNKNOWN_SLOT))
	{
		sTeammate& teammate = mTeam[aTeammateIndex];
		const auto& slotIt = teammate.itemSlot.find(aSlot);
		if (slotIt != teammate.itemSlot.end())
		{
			std::string& equipedItem = slotIt->second;
			const auto& items = DM->getData().items;
			auto itemIt = items.find(equipedItem);
			if (itemIt != items.end())
			{
				teammate.damage -= itemIt->second.damage;
				mPassiveDamage -= itemIt->second.damage;
				mInventory.push_back(equipedItem);
				equipedItem.clear();
				result = true;
			}
		}
	}

	return result;
}

bool Player::equipItem(eSlotID aSlot, int aItemIndex)
{
	bool result = false;

	if ((aSlot != eSlotID::UNKNOWN_SLOT) &&
		(aItemIndex > -1) &&
		(aItemIndex < mInventory.size()))
	{
		std::string& itemID = mInventory[aItemIndex];
		const auto& slotIt = mEquipedItems.find(aSlot);
		if (slotIt != mEquipedItems.end())
		{
			const auto& items = DM->getData().items;
			auto itemIt = items.find(itemID);
			if (itemIt != items.end())
			{
				std::string& equipedItem = slotIt->second;
				if (!equipedItem.empty())
				{
					auto equipedItemIt = items.find(equipedItem);
					if (equipedItemIt != items.end())
					{
						mClickDamage -= equipedItemIt->second.damage;
						mInventory.push_back(equipedItem);
					}
				}

				const sDamage& itemDamage = itemIt->second.damage;
				mClickDamage += itemDamage;
				equipedItem = itemID;
				mInventory.erase(mInventory.begin() + aItemIndex);
				result = true;
			}	
		}
	}

	return result;
}

bool Player::clearSlot(eSlotID aSlot)
{
	bool result = false;

	if ((aSlot != eSlotID::UNKNOWN_SLOT))
	{
		const auto& slotIt = mEquipedItems.find(aSlot);
		if (slotIt != mEquipedItems.end())
		{
			const auto& items = DM->getData().items;
			std::string& equipedItemID = slotIt->second;
			auto itemIt = items.find(equipedItemID);
			if (itemIt != items.end())
			{
				mClickDamage -= itemIt->second.damage;
				mInventory.push_back(equipedItemID);
				equipedItemID.clear();
				result = true;
			}
		}
	}

	return result;
}

bool Player::sellItem(int aItemIndex)
{
	bool result = false;

	if ((aItemIndex > -1) &&
		aItemIndex < mInventory.size())
	{
		const std::string& itemID = mInventory[aItemIndex];
		const auto& items = DM->getData().items;
		const auto& itemIt = items.find(itemID);
		if (itemIt != items.end())
		{
			addGold(itemIt->second.price);
			mInventory.erase(mInventory.begin() + aItemIndex);
			result = true;
		}
	}

	return result;
}

const std::string& Player::getEquipedItem(eSlotID aSlotID)
{
	return mEquipedItems[aSlotID];
}

const std::string& Player::getEquipedItem(int aTeammateIndex, eSlotID aSlotID)
{
	return mTeam[aTeammateIndex].itemSlot[aSlotID];
}

const sDamage& Player::getClickDamage() const
{
	return mClickDamage;
}

const sDamage& Player::getTeammateDamage(int aTeammateIndex) const
{
	return mTeam[aTeammateIndex].damage;
}

const sDamage& Player::getPassiveDamage() const
{
	return mPassiveDamage;
}

int Player::getCurrentHirePriceMultiplier() const
{
	int result = 1;
	const auto& levelsData = DM->getData().levels;
	for (auto& levelKills : mKills)
	{
		const std::string& levelID = levelKills.first;
		const auto levelIt = levelsData.find(levelID);
		if (levelIt != levelsData.end())
		{
			result *= levelIt->second.hirePriceMultiplier;
		}
	}

	return result;
}

int Player::getTeamSize() const
{
	return mTeam.size();
}

int Player::getMaxTeamSize() const
{
	int result = 0;
	const auto& levelsData = DM->getData().levels;
	for (auto& levelKills : mKills)
	{
		const std::string& levelID = levelKills.first;
		const auto levelIt = levelsData.find(levelID);
		if (levelIt != levelsData.end())
		{
			result += levelIt->second.additionalTeammates;
		}
	}

	return result;
}

bool Player::isNeedTeammate() const
{
	return getTeamSize() < getMaxTeamSize();
}

bool Player::spendGold(const unsigned int aAmount)
{
	bool result = false;
	if (mGold >= aAmount)
	{
		mGold -= aAmount;
		result = true;
	}
	return result;
}

bool Player::isGoldEnough(const unsigned int aAmount)
{
	return mGold >= aAmount;
}

void Player::hireTeammate()
{
	mTeam.push_back(sTeammate());
}

void Player::addItem(const std::string& aItemID)
{
	mInventory.push_back(aItemID);
}
