#include "PopUpOptions.h"
#include "DataManager.h"
#include "ViewBuilder.h"

PopUpOptions::PopUpOptions()
	: mFlagSelected(nullptr)
{
}

PopUpOptions::~PopUpOptions()
{
}

bool PopUpOptions::init()
{
	bool result = false;

	if (Parent::init())
	{
		result = true;
		
		const std::string& view = DM->getViewById("MAIN_MENU_OPTIONS");
		result &= ViewBuilder::loadFromJson(this, view);

		initButtons();

		mFlagSelected = getChildByName("flag_selected");
		if (mFlagSelected != nullptr)
		{
			cocos2d::Node* flagLocaleButton = getChildByName(DM->getSettings().locale);
			if (flagLocaleButton != nullptr)
			{
				mFlagSelected->setPosition(flagLocaleButton->getPosition());
			}
		}
	}

	return result;
}

void PopUpOptions::initButtons()
{
	std::vector<cocos2d::ui::Button*> buttons;
	buttons.push_back(getChildByName<cocos2d::ui::Button*>("btnOK"));
	buttons.push_back(getChildByName<cocos2d::ui::Button*>("en"));
	buttons.push_back(getChildByName<cocos2d::ui::Button*>("ru"));

	for (auto& button : buttons)
	{
		if (button != nullptr)
		{
			button->addTouchEventListener(CC_CALLBACK_2(PopUpOptions::onButtonTouched, this));
		}
	}
}

void PopUpOptions::onButtonTouched(cocos2d::Ref* aSender, cocos2d::ui::Widget::TouchEventType aEvent)
{
	if (aEvent == cocos2d::ui::Widget::TouchEventType::ENDED && mIsActive)
	{
		cocos2d::Node* btn = static_cast<cocos2d::Node*>(aSender);
		const std::string& btnName = btn->getName();

		if(btnName == "btnOK")
		{
			hide();
			DM->saveSettings();
		}
		else if(btnName == "en" ||
				btnName == "ru")
		{
			mFlagSelected->setPosition(btn->getPosition());
			std::string& locale = DM->getSettings().locale;
			if (locale != btnName)
			{
				locale = btnName;
				//reload strings
			}
		}
	}
}
