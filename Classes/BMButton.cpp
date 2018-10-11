#include "BMButton.h"
#include "DataManager.h"

BMButton::BMButton()
{

}

//bool BMButton::init()
//{
//	bool result = false;
//	if (Parent::init())
//	{
//		result = true;
//		mTitle = cocos2d::Label::create();
//	}
//
//	return result;
//}

BMButton* BMButton::create(	const std::string& aNormalFrameNameID, 
							const std::string& aSelectedFrameNameID, 
							const std::string& aDisabledFrameNameID)
{
	const sData& data = DM->getData();

	std::string normalFrameName;
	const auto normalFrameNameIt = data.images.find(aNormalFrameNameID);
	if (normalFrameNameIt != data.images.end())
	{
		normalFrameName = normalFrameNameIt->second;
	}

	std::string selectedFrameName;
	if (!aSelectedFrameNameID.empty())
	{
		const auto selectedFrameNameIt = data.images.find(aNormalFrameNameID);
		if (selectedFrameNameIt != data.images.end())
		{
			selectedFrameName = selectedFrameNameIt->second;
		}
	}
	
	std::string disabledFrameName;
	if (!aDisabledFrameNameID.empty())
	{
		const auto disabledFrameNameIt = data.images.find(aNormalFrameNameID);
		if (disabledFrameNameIt != data.images.end())
		{
			disabledFrameName = disabledFrameNameIt->second;
		}
	}

	BMButton* btn = new(std::nothrow)BMButton;
	if (btn && btn->init(normalFrameName, selectedFrameName, disabledFrameName, cocos2d::ui::Widget::TextureResType::PLIST))
	{
		btn->autorelease();
		return btn;
	}
	CC_SAFE_DELETE(btn);
	return nullptr;
}

void BMButton::setTitle(const std::string& aTitleStringID, const std::string& aFontID)
{
	const sData& data = DM->getData();
	const auto strIt = data.strings.find(aTitleStringID);
	const auto fontIt = data.fonts.find(aFontID);
	if ((strIt != data.strings.end()) && (fontIt != data.fonts.end()))
	{
		cocos2d::Label* title = cocos2d::Label::createWithBMFont(fontIt->second, strIt->second);
		setTitleLabel(title);
	}
}
