#include "MainMenuScene.h"
#include "BattleScene.h"
#include"AudioManager.h"
USING_NS_CC;

MainMenuScene* MainMenuScene::create()
{
    MainMenuScene* ret = new MainMenuScene();
    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        delete ret;
        return nullptr;
    }
}

bool MainMenuScene::init()
{
     if (!Scene::init()) { 
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    createBackground();
    // 创建开始游戏按钮
    auto startButton = MenuItemLabel::create(Label::createWithTTF("GAME START","fonts/Clash_Regular.otf", 36), CC_CALLBACK_1(MainMenuScene::onStartGame, this));
    startButton->setPosition(visibleSize.width / 2, visibleSize.height / 3);

    auto menu = Menu::create(startButton, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);


    //播放背景音乐
    AudioManager::getInstance()->playBgMusic("music/sounds/menu_03.wav");

    float bgVol = SimpleAudioEngine::getInstance()->getBackgroundMusicVolume();

    //确认音量设置无异常
    float effectVol = SimpleAudioEngine::getInstance()->getEffectsVolume();
    CCLOG("背景音乐音量：%.2f，音效音量：%.2f", bgVol, effectVol);
    return true;
}

void MainMenuScene::onStartGame(Ref* sender)
{
    // 切换到战斗场景
    AudioManager::getInstance()->stopBgMusic();
    Director::getInstance()->replaceScene(BattleScene::create());
}

//用于主程序调用
Scene* MainMenuScene::createScene()
{
    return MainMenuScene::create(); 
}



//创建按钮
void MainMenuScene::createButton()
{
    return;
}


//*创建主界面背景图
void MainMenuScene::createBackground()
{
    background = Sprite::create("Images/background/base.webp");
    auto TrainingCamp = Sprite::create("Images/background/TRAINING CAMP.png");
    if (!background) { 
        CCLOG("主界面背景图加载失败！检查路径：Images/background/base.webp");
        return;
    }
    CCLOG("主界面背景图加载成功，原始尺寸：%.0f x %.0f", background->getContentSize().width, background->getContentSize().height);
    Size winSize = Director::getInstance()->getVisibleSize(); // 主界面用窗口尺寸
    float scaleX = winSize.width / background->getContentSize().width;
    float scaleY = winSize.height / background->getContentSize().height;
    background->setScale(MAX(scaleX, scaleY)); // 取最大比例

    CCLOG("背景缩放后尺寸：%.0f x %.0f",
        background->getContentSize().width * scaleX,
        background->getContentSize().height * scaleY);

    //用窗口尺寸计算位置，锚点适配主界面
    background->setAnchorPoint(Vec2(0.5f, 0.5f)); // 锚点设中心
    background->setPosition(winSize.width / 2, winSize.height / 2); // 背景居中

  
    TrainingCamp->setAnchorPoint(Vec2(0.5f, 0.5f));
    TrainingCamp->setPosition(winSize.width / 2, winSize.height*6 / 10);

    addChild(background, 0); // 层级0，最底层
    addChild(TrainingCamp, 1);
}