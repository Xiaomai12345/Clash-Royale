#include "MainMenuScene.h"
#include "BattleScene.h"
#include"CardSelectingScene.h"
#include"AudioManager.h"
#include"Button.h"
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

    createBackground();//创建背景

    createUI();//放置UI

    createButton();//放置按钮

    auto audio = SimpleAudioEngine::getInstance();
    if(!audio->isBackgroundMusicPlaying())
        playMusic();//播放音乐

    return true;
}

// 切换到战斗场景
void MainMenuScene::onStartGame(Ref* sender)
{   
    auto cardMgr = CardManager::getInstance();
    int selectedCount = (int)cardMgr->getSelectedCards().size();

    // 校验是否选满8张
    if (selectedCount < 8)
    {
        Label* tipLabel = Label::createWithTTF("At least  \n 8 Cards are needed!", "fonts/Clash_Bold.otf", 60);
        tipLabel->setColor(Color3B(255, 0, 0)); // 红色提示
        tipLabel->setPosition(Vec2(Director::getInstance()->getVisibleSize().width/2, 200));
        this->addChild(tipLabel, 100);
        //2秒后自动移除提示
        tipLabel->runAction(Sequence::create(DelayTime::create(2.0f), RemoveSelf::create(), nullptr));

        CCLOG("错误：当前仅选了 %d 张卡牌，需选满8张！", selectedCount);
        return; // 不执行后续战斗场景切换
    }

    AudioManager::getInstance()->stopBgMusic();
    Director::getInstance()->replaceScene(BattleScene::create());
}

//切换到选卡界面
void MainMenuScene::toCardSelecting(Ref* sender)
{
    Director::getInstance()->replaceScene(CardSelectingScene::create());
}
//用于主程序调用
Scene* MainMenuScene::createScene()
{
    return MainMenuScene::create(); 
}

void MainMenuScene::playMusic()
{
    //播放背景音乐
    AudioManager::getInstance()->playBgMusic("music/sounds/menu_03.wav");

    float bgVol = SimpleAudioEngine::getInstance()->getBackgroundMusicVolume();

    //确认音量设置无异常
    float effectVol = SimpleAudioEngine::getInstance()->getEffectsVolume();
    CCLOG("背景音乐音量：%.2f，音效音量：%.2f", bgVol, effectVol);
}


//创建按钮
void MainMenuScene::createButton()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 screenCenter = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    // 创建开始游戏按钮
    auto startBtn = GameButton::create(
        "Images/button/start_btn.png",    // 正常态图片路径
        "Images/button/start_btn_selected.png",   // 按下态图片路径
        "Images/button/start_btn_disabled.png",  // 禁用态图片路径
        "BATTLE",                         // 按钮文字
        "fonts/Clash_Regular.otf",            // 字体路径
        36,                                   // 文字大小
        [=]() { 
            CCLOG("点击了游戏开始按钮！");
            this->onStartGame(nullptr);
        }
    );

    auto MainBtn = GameButton::create(
        "Images/button/Main.png",    // 正常态图片路径
        "Images/button/Main_selected.png",   // 按下态图片路径
        "Images/button/Main_disabled.png",  // 禁用态图片路径
        "",                         // 按钮文字
        "",            // 字体路径
        0,                                   // 文字大小
        nullptr
    );

    //创建切换到换卡页面的按钮
    auto CradtBtn = GameButton::create(
        "Images/button/cards.png",    // 正常态图片路径
        "Images/button/cards_selected.png",   // 按下态图片路径
        "Images/button/cards_disabled.png",  // 禁用态图片路径
        "",                         // 按钮文字
        "",            // 字体路径
        0,                                   // 文字大小
        [=]() {
            CCLOG("点击了切换卡牌页面按钮！");
            this->toCardSelecting(nullptr);
        }
    );

    // 设置按钮位置（屏幕居中，向上偏移100px）
    startBtn->setPosition(Vec2(screenCenter.x, screenCenter.y - 200));
    startBtn->setScale(0.9);

    MainBtn->setPosition(Vec2(screenCenter.x, screenCenter.y * 0.138));
    MainBtn->setScale(0.5);
    MainBtn->setEnabled(false);

    CradtBtn->setPosition(Vec2(screenCenter.x-180, screenCenter.y*0.08));
    CradtBtn->setScale(0.5);

    this->addChild(startBtn);
    this->addChild(CradtBtn);
    this->addChild(MainBtn);
    return;
}


//*创建主界面背景图
void MainMenuScene::createBackground()
{
    background = Sprite::create("Images/background/base.webp");
    if (!background) { 
        CCLOG("主界面背景图加载失败！检查路径：Images/background/base.webp");
        return;
    }
    CCLOG("主界面背景图加载成功，原始尺寸：%.0f x %.0f", background->getContentSize().width, background->getContentSize().height);
    Size winSize = Director::getInstance()->getVisibleSize(); // 主界面用窗口尺寸
    float scaleX = winSize.width / background->getContentSize().width;
    float scaleY = winSize.height / background->getContentSize().height;
    background->setScale(MIN(scaleX, scaleY)); // 取最大比例

    CCLOG("背景缩放后尺寸：%.0f x %.0f",
        background->getContentSize().width * scaleX,
        background->getContentSize().height * scaleY);

    //用窗口尺寸计算位置，锚点适配主界面
    background->setAnchorPoint(Vec2(0.5f, 0.5f)); // 锚点设中心
    background->setPosition(winSize.width / 2, winSize.height / 2); // 背景居中

    addChild(background, 0); // 层级0，最底层
}


//放置主界面UI
void MainMenuScene::createUI()
{
    auto TrainingCamp = Sprite::create("Images/MenuUI/TRAINING CAMP.png");
    auto FreeChest = Sprite::create("Images/MenuUI/FreeChest.png");
    auto SilverChest = Sprite::create("Images/MenuUI/SilverChest.png");
    auto NuevoCofre1 = Sprite::create("Images/MenuUI/NuevoCofre1.png");
    auto NuevoCofre2 = Sprite::create("Images/MenuUI/Nuevo Cofre2.png");

    Size winSize = Director::getInstance()->getVisibleSize(); // 用窗口尺寸

    TrainingCamp->setScale(1.3);
    FreeChest->setScale(0.4);
    SilverChest->setScale(0.4);
    NuevoCofre1->setScale(0.4);
    NuevoCofre2->setScale(0.4);
    
    TrainingCamp->setAnchorPoint(Vec2(0.5f, 0.5f));
    TrainingCamp->setPosition(winSize.width / 2, winSize.height * 59 / 100);

    FreeChest->setAnchorPoint(Vec2(0.5f, 0.5f));
    FreeChest->setPosition(winSize.width / 6, winSize.height * 18 / 100);

    SilverChest->setAnchorPoint(Vec2(0.5f, 0.5f));
    SilverChest->setPosition(winSize.width*9 / 24, winSize.height * 18 / 100);

    NuevoCofre1->setAnchorPoint(Vec2(0.5f, 0.5f));
    NuevoCofre1->setPosition(winSize.width*29 / 48, winSize.height * 18 / 100);

    NuevoCofre2->setAnchorPoint(Vec2(0.5f, 0.5f));
    NuevoCofre2->setPosition(winSize.width*39 / 48, winSize.height * 18 / 100);

    addChild(TrainingCamp, 1);
    addChild(FreeChest, 1);
    addChild(SilverChest, 1);
    addChild(NuevoCofre1, 1);
    addChild(NuevoCofre2, 1);
}