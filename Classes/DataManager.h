#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "dataTypes.h"

class DataManager
{
private:
	sData mData;
	sSettings mSettings;

	DataManager();

	void loadMainConfig(const std::string& aPath);
	void loadSettings();
	void loadImages(const std::string& aPath);
	void loadFonts(const std::string& aPath);
	void loadStrings(const std::string& aPath);
	void loadViews(const std::string& aPath);

public:
	static DataManager* getInstance();
	const sData& getData() const;
	sSettings& getSettings();
};

#define DM DataManager::getInstance()

#endif //DATAMANAGER_H