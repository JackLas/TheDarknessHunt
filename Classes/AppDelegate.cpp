#include "AppDelegate.h"
#include "MainMenuScene.h"
#include "DataManager.h"

// #define USE_AUDIO_ENGINE 1
// #define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

USING_NS_CC;


AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
#endif
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil,multisamplesCount
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() 
{
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		const cocos2d::Size& windowSize = DM->getData().desktopResolution;
        glview = GLViewImpl::createWithRect("The Darkness Hunt", 
			cocos2d::Rect(0, 0, windowSize.width, windowSize.height));
#else
        glview = GLViewImpl::create("The Darkness Hunt");
#endif
        director->setOpenGLView(glview);
    }

#if defined(COCOS2D_DEBUG) || COCOS2D_DEBUG == 1
    director->setDisplayStats(true);
#endif

    director->setAnimationInterval(1.0f / 60);

	const cocos2d::Size& desRes = DM->getData().designResolution;
	glview->setDesignResolutionSize(desRes.width, desRes.height, ResolutionPolicy::SHOW_ALL);

    register_all_packages();

	cocos2d::SpriteFrameCache* cache = cocos2d::SpriteFrameCache::getInstance();
	int packN = 0;
	std::string currentPack = "images/images-" + std::to_string(packN) + ".plist";
	while (cocos2d::FileUtils::getInstance()->isFileExist(currentPack))
	{
		cache->addSpriteFramesWithFile(currentPack);
		currentPack = "images/images-" + std::to_string(++packN) + ".plist";
	}

    // create a scene. it's an autorelease object
    auto scene = MainMenuScene::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() 
{
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() 
{
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}
