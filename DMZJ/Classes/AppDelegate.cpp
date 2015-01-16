#include "AppDelegate.h"
#include "RootWindow.h"
#include "umSDK.h"
USING_NS_CC;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate() 
{
    
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CAApplication* pDirector = CAApplication::getApplication();
    
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);

    // run
    pDirector->runWindow(RootWindow::getInstance());
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    MOBCLICKCPP_START_WITH_APPKEY_AND_CHANNEL("5462c2ecfd98c52a84000d91", "000000");
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    MOBCLICKCPP_START_WITH_APPKEY_AND_CHANNEL("54696a53fd98c520b1000f3a", "000000");
#endif
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CAImageCache::sharedImageCache()->removeUnusedImages();
    CAApplication::getApplication()->stopAnimation();
    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CAApplication::getApplication()->startAnimation();
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
