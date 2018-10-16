#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "dataTypes.h"

class DataManager
{
private:
	sData mData;
	sSettings mSettings;

	const std::string mDefaultStr;

	DataManager();

	void loadMainConfig(const std::string& aPath);
	void loadSettings();
	void loadResources(const std::string& aPath, std::map<std::string, std::string>& aContainer);
	const std::string& getResourceById(const std::string& aID, std::map<std::string, std::string>& aContainer);

public:
	static DataManager* getInstance();
	const sData& getData() const;
	sSettings& getSettings();
	void saveSettings();

	const std::string& getStringById(const std::string& aID);
	const std::string& getFrameNameById(const std::string& aID);
	const std::string& getFontById(const std::string& aID);
	const std::string& getViewById(const std::string& aID);
};

#define DM DataManager::getInstance()

#endif //DATAMANAGER_H