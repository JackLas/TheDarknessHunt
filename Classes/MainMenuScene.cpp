#include "MainMenuScene.h"

MainMenuScene::MainMenuScene()
{
}

bool MainMenuScene::init()
{
	bool result = false;
	
	if (Parent::init())
	{
		result = true;
	}

	return result;
}