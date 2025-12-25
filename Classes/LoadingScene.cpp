#include "LoadingScene.h"
#include "MainMenuScene.h"
#include "ui/CocosGUI.h"
#include <functional>
USING_NS_CC;

Scene* LoadingScene::createScene()
{
    return LoadingScene::create();
}

bool LoadingScene::init()
{
    if (!Scene::init()) {
        return false;
    }

    
    // 创建加载背景
    createLoadingBackground();
    //创建进度条
    createLoadingBar();
    //预加载资源
    preloadResources();


    return true;
}

// 加载加载图
void LoadingScene::createLoadingBackground()
{
    loadingBg = Sprite::create("Images/background/enter.jpg");
    if (!loadingBg) {
        CCLOG("加载场景背景图加载失败！检查路径：Images/background/enter.jpg");
        return;
    }
    CCLOG("加载场景背景图加载成功");

    // 动态适配窗口
    const Size winSize = Director::getInstance()->getVisibleSize();
    const float scaleX = winSize.width / loadingBg->getContentSize().width;
    const float scaleY = winSize.height / loadingBg->getContentSize().height;
    loadingBg->setScale(MAX(scaleX, scaleY)); 

    // 锚点+位置
    loadingBg->setAnchorPoint(Vec2(0.5f, 0.5f));
    loadingBg->setPosition(winSize.width / 2, winSize.height / 2);
    this->addChild(loadingBg, 0); // 层级0，最底层
}

// 预加载主界面/战斗场景的核心资源
void LoadingScene::preloadResources()
{
    // 预加载字体
    Director::getInstance()->getTextureCache()->addImageAsync("fonts/Clash_Regular.ttf", std::bind(&LoadingScene::onResLoaded, std::placeholders::_1,this));
    //预加载主界面背景图
    Director::getInstance()->getTextureCache()->addImageAsync("Images/background/base.webp", std::bind(&LoadingScene::onResLoaded, std::placeholders::_1, this));
    // 预加载战斗场景背景图
    Director::getInstance()->getTextureCache()->addImageAsync("Images/background/battlefield.jpg", std::bind(&LoadingScene::onResLoaded, std::placeholders::_1, this));

    CCLOG("资源预加载完成！");
}

// 跳转到主界面，加淡入淡出动画
void LoadingScene::jumpToMainMenu(float dt)
{
    auto mainMenuScene = MainMenuScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5f, mainMenuScene));
    CCLOG("从加载场景跳转到主界面！");
}

//创建加载条
void LoadingScene::createLoadingBar() {
    auto loadingBarbg = Sprite::create("Images/LoadingUI/LBbg.png");
    loadingBar = cocos2d::ui::LoadingBar::create("Images/LoadingUI/LoadingBar.png");
    if (!loadingBar||!loadingBarbg) {
        CCLOG("加载场景进度条加载失败！检查路径：Images/LoadingUI/LoadingBar.png");
        return;
    }
    CCLOG("加载场景进度条加载成功");

    // 锚点+位置
    loadingBar->setAnchorPoint(Vec2(0.5f, 0.5f));
    const Size winSize = Director::getInstance()->getVisibleSize();
    loadingBar->setPosition(Vec2(winSize.width / 2, winSize.height * 0.04f));
    loadingBar->setScale(1.1,0.75);
    loadingBar->setDirection(cocos2d::ui::LoadingBar::Direction::LEFT);

    loadingBarbg->setAnchorPoint(Vec2(0.5f, 0.5f));
    loadingBarbg->setPosition(Vec2(winSize.width / 2, winSize.height * 0.04f));
    loadingBarbg->setScale(0.8, 0.8);

    //制造滑动效果
    loadingBar->setPercent(0);
    totalResCount = 3;
    loadedResCount = 0;
    this->addChild(loadingBarbg, 1);
    this->addChild(loadingBar,2);
}

//协调进度条
void LoadingScene::onResLoaded(cocos2d::Texture2D* texture, LoadingScene* self) {
    self->loadedResCount++;
    float progress = (float)self->loadedResCount / (float)self->totalResCount * 80;
    self->loadingBar->setPercent(progress);

    //演示效果
    if (self->loadedResCount == self->totalResCount) {
        auto set80Percent = CallFunc::create([=]() {
            self->loadingBar->setPercent(80);
            });
        auto delay = DelayTime::create(1.0f);
        auto progressTo100 = ProgressTo::create(0.5f, 100);
        auto jumpToMenu = CallFunc::create([self]() {
            self->jumpToMainMenu(0.0f);
        });
        auto progressSeq = Sequence::create(
            set80Percent,  // 先设80%
            delay,       // 停1秒
            progressTo100, // 0.5秒渐变到100%
            DelayTime::create(0.5f), // 到100%后缓冲0.5秒
            CallFunc::create([=]() { self->jumpToMainMenu(0.0f); }),
            nullptr
        );
        self->loadingBar->runAction(progressSeq);
    }
    return;
}