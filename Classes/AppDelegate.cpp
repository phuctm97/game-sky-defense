#include "AppDelegate.h"
#include "MainScene.h"

#include "Include.h"

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(2048, 1536);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("SkyDefense", Rect(0, 0, mediumResolutionSize.width, mediumResolutionSize.height));
#else
        glview = GLViewImpl::create("SkyDefense");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    //director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    std::vector<std::string> resourcesPaths;

    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
    Size frameSize = glview->getFrameSize();
    // if the frame's height is larger than the height of medium size.
    if (frameSize.height > mediumResolutionSize.height)
    {
        resourcesPaths.push_back("ipadhd");
    }
    // if the frame's height is larger than the height of small size.
    else if (frameSize.height > smallResolutionSize.height)
    {        
        resourcesPaths.push_back("ipad");
    }
    // if the frame's height is smaller than the height of medium size.
    else
    {        
        resourcesPaths.push_back("iphone");
    }

    register_all_packages();
    director->setContentScaleFactor(MIN(frameSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    FileUtils::getInstance()->setSearchResolutionsOrder(resourcesPaths);

    //preload audios
    AUDIO_PRELOAD("background.mp3");
    AUDIO_PRELOAD("boom.mp3");
    AUDIO_PRELOAD("bombFail.mp3");
    AUDIO_PRELOAD("bombRelease.mp3");
    AUDIO_PRELOAD("health.mp3");
    AUDIO_PRELOAD("fire_truck.mp3");
    AUDIO_PRELOAD("pew.mp3");


    // create a scene. it's an autorelease object
	Scene* scene = MainScene::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
