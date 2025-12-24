#include "AppDelegate.h"
#include "DataManager.h" 
#include "BattleScene.h"
#include"MainMenuScene.h"
#include"LoadingScene.h"
// 如果使用预编译头，请包含它
// #include "AppDelegate.h"

USING_NS_CC;

AppDelegate::AppDelegate()
    : _director(nullptr)
    , _glview(nullptr)
{
}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // 初始化导演类
    _director = Director::getInstance();
    _glview = _director->getOpenGLView();
    if (!_glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        _glview = GLViewImpl::createWithRect("ClashRoyale", cocos2d::Rect(0, 0, 1308 * 0.8 * 0.85, 1732 * 0.8));
#else
        _glview = GLViewImpl::create("ClashRoyale");
#endif
        _director->setOpenGLView(_glview);
    }
    //加载卡牌Json配置
    DataManager::getInstance()->loadCardConfig("data\\card_config.json");
    // 设置FPS显示
    _director->setDisplayStats(true);

    // 设置FPS
    _director->setAnimationInterval(1.0f / 60);

    // 创建第一个场景（主界面）
    auto scene = LoadingScene::createScene();
    _director->runWithScene(scene);
    return true;
}

void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();
}

void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();
}
//退出时销毁实例
void AppDelegate::applicationExit() {
    DataManager::destroyInstance();
}