#ifndef BMBUTTON_H
#define BMBUTTON_H

#include "cocos2d.h"
#include "ui/UIButton.h"

class BMButton: public cocos2d::ui::Button
{
private:
	typedef cocos2d::ui::Button Parent;
	//cocos2d::Label* mTitle;
	std::string mBMFontPath;

	BMButton();
protected:
public:
	//CREATE_FUNC(BMButton);
	//void setSpriteByFrameName(const std::string& aFrameName);

	static BMButton* create(const std::string& aNormalFrameNameID, 
							const std::string& aSelectedFrameNameID = "", 
							const std::string& aDisabledFrameNameID = "");
	void setTitle(const std::string& aTitleStringID, const std::string& aFontID);
};

#endif //BMBUTTON_H
