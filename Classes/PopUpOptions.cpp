#include "PopUpOptions.h"
#include "DataManager.h"
#include "ViewBuilder.h"

PopUpOptions::PopUpOptions()
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
		const sData& data = DM->getData();
		const auto viewIt = data.views.find("MAIN_MENU_OPTIONS");
		if (viewIt != data.views.end())
		{
			result &= ViewBuilder::loadFromJson(this, viewIt->second);
		}

		initButtons();
	}

	return result;
}

void PopUpOptions::initButtons()
{
	std::vector<cocos2d::ui::Button*> buttons;
	buttons.push_back(getChildByName<cocos2d::ui::Button*>("btnOK"));

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

		if (btnName == "btnOK")
		{
			hide();
		}
	}
}
