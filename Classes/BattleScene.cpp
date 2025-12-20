#include "BattleScene.h"
#include "Battlefield.h"
#include "ManaSystem.h"
#include "ManaBar.h"

USING_NS_CC;

Scene* BattleScene::createScene()
{
    return BattleScene::create();
}

bool BattleScene::init()
{
    if (!Scene::init())
        return false;

    // 初始化游戏状态
    _gameStarted = false;
    _gameEnded = false;
    _gameTime = 0.0f;
    _totalGameTime = 180.0f;
    _playerCrowns[0] = _playerCrowns[1] = 0;
    _isPlayer1 = true;
    _selectedCard = nullptr;
    _cardGhost = nullptr;

    // 创建战场
    _battlefield = Battlefield::create();
    _battlefield->setupBattlefield(1);
    addChild(_battlefield, 0);

    // 设置UI
    setupUI();

    // 设置输入
    setupInput();

    // 初始化圣水系统
    ManaSystem::getInstance()->init(5.0f, 10.0f, 0.8f);


    // 开始游戏
    startGame();

    // 开启更新
    scheduleUpdate();

    return true;
}

void BattleScene::setupUI()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // 创建圣水条
    _manaBar = ManaBar::create();
    _manaBar->setPosition(visibleSize.width / 2, visibleSize.height - 50);
    addChild(_manaBar, 100);

    // 创建倒计时标签
    auto countdownLabel = Label::create("Time Left :", "fonts/Clash_Regular.otf", 20);
    auto countdown = Label::createWithSystemFont("3:00", "fonts/Clash_Regular.otf", 48);
    countdown->setPosition(visibleSize.width-80, visibleSize.height - 70);
    countdownLabel->setPosition(visibleSize.width-80, visibleSize.height-35);
    countdown->setTextColor(Color4B::WHITE);
    countdownLabel->setTextColor(Color4B::WHITE);
    countdown->setTag(1001);
    addChild(countdown, 100);
    addChild(countdownLabel, 100);
    // 使用 DrawNode 绘制矩形框
    auto drawNode = DrawNode::create();

    // 绘制填充矩形
    drawNode->drawSolidRect(
        Vec2(visibleSize.width - 140, visibleSize.height - 95),           // 左下角
        Vec2(visibleSize.width - 20, visibleSize.height - 20),           // 右上角
        Color4F(0, 0, 0,0.5f )  // 50%黑色填充
    );

    // 绘制边框
    drawNode->drawRect(
        Vec2(visibleSize.width - 140, visibleSize.height - 95),           // 左下角
        Vec2(visibleSize.width-20, visibleSize.height - 20),           // 右上角
        Color4F(0, 0, 0, 1.0f)    // 黑色边框
    );

    addChild(drawNode, 90);

    //绘制圣水倍率"X1"
    auto closeLabel = Label::createWithSystemFont("\\", "Arial Bold", 48);
    auto closeLabe2 = Label::createWithSystemFont("/", "Arial Bold", 48);
    closeLabel->setTextColor(Color4B(128,0,128,255));
    closeLabel->setPosition(visibleSize.width - 90, visibleSize.height - 120);
    addChild(closeLabel,100);
    closeLabe2->setTextColor(Color4B(128, 0, 128, 255));
    closeLabe2->setPosition(visibleSize.width - 90, visibleSize.height - 120);
    addChild(closeLabe2, 100);

    //// 调试按钮
    //auto debugButton = MenuItemLabel::create(
    //    Label::createWithSystemFont("Debug", "Arial", 24),
    //    [this](Ref* sender) {
    //        _battlefield->enableDebugDraw(!_battlefield->isDebugDrawEnabled());
    //    }
    //);

    //debugButton->setPosition(Vec2(visibleSize.width - 80, 50));

    //auto menu = Menu::create(debugButton, nullptr);
    //menu->setPosition(Vec2::ZERO);
    //addChild(menu, 200);
}

void BattleScene::setupInput()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    listener->onTouchBegan = CC_CALLBACK_2(BattleScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(BattleScene::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(BattleScene::onTouchEnded, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool BattleScene::onTouchBegan(Touch* touch, Event* event)
{
    if (!_gameStarted || _gameEnded)
        return false;

    Vec2 touchPos = touch->getLocation();


    // 创建拖拽虚影
    if (!_cardGhost)
    {
        _cardGhost = Sprite::create();
        auto color = LayerColor::create(Color4B(255, 255, 255, 100), 100, 140);
        _cardGhost->addChild(color);
        addChild(_cardGhost, 150);
    }

    _cardGhost->setPosition(touchPos);
    _cardGhost->setVisible(true);

    return true;
}



void BattleScene::startGame()
{
    _gameStarted = true;
    _gameTime = 0.0f;

    CCLOG("Game started!");
}

void BattleScene::update(float delta)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Scene::update(delta);

    if (!_gameStarted || _gameEnded)
        return;

    // 更新游戏时间
    _gameTime += delta;

    // 更新倒计时显示
    int remainingTime = (int)(_totalGameTime - _gameTime);
    if (remainingTime < 0) remainingTime = 0;

    int minutes = remainingTime / 60;
    int seconds = remainingTime % 60;

    if (minutes >= 1)
    {
        auto _manaMutiple = Label::createWithSystemFont("1", "Arial Bold", 48);
        _manaMutiple->setTextColor(Color4B(128, 0, 128, 255));
        _manaMutiple->setPosition(visibleSize.width - 60, visibleSize.height - 120);
        addChild(_manaMutiple, 100);
    }
    else if (minutes >= 0 && minutes < 1)
    {
        auto _manaMutiple = Label::createWithSystemFont("2", "Arial Bold", 48);
        _manaMutiple->setTextColor(Color4B(128, 0, 128, 255));
        _manaMutiple->setPosition(visibleSize.width - 60, visibleSize.height - 120);
        addChild(_manaMutiple, 100);
    }
    auto countdownLabel = dynamic_cast<Label*>(getChildByTag(1001));
    if (countdownLabel)
    {
        countdownLabel->setString(StringUtils::format("%2d:%02d", minutes, seconds));
    }

    // 检查游戏结束条件
    if (remainingTime <= 0)
    {
        endGame(_playerCrowns[0] > _playerCrowns[1]);
        return;
    }

    // 更新圣水系统
    ManaSystem::getInstance()->update(delta);

    // 更新皇冠显示
    for (int i = 0; i < 2; i++)
    {
        auto crownNode = getChildByTag(2001 + i);
        if (crownNode)
        {
            auto label = dynamic_cast<Label*>(crownNode->getChildByTag(10));
            if (label)
            {
                label->setString(StringUtils::format("%d", _playerCrowns[i]));
            }
        }
    }
}

void BattleScene::endGame(bool isPlayer1Win)
{
    if (_gameEnded)
        return;

    _gameEnded = true;
    _gameStarted = false;

    // 显示游戏结束界面
    auto visibleSize = Director::getInstance()->getVisibleSize();

    auto resultLayer = LayerColor::create(Color4B(0, 0, 0, 180),
        visibleSize.width,
        visibleSize.height);
    addChild(resultLayer, 1000);

    // 结果文本
    std::string resultText = isPlayer1Win ? "Player 1 Wins!" : "Player 2 Wins!";
    auto resultLabel = Label::createWithSystemFont(resultText, "Arial", 72);
    resultLabel->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 100);
    resultLabel->setTextColor(Color4B::YELLOW);
    resultLayer->addChild(resultLabel);

    // 返回按钮
    auto returnButton = MenuItemLabel::create(
        Label::createWithSystemFont("Return to Menu", "Arial", 32),
        [](Ref* sender) {
            Director::getInstance()->popScene();
        }
    );

    returnButton->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 100);

    auto menu = Menu::create(returnButton, nullptr);
    menu->setPosition(Vec2::ZERO);
    resultLayer->addChild(menu);

    CCLOG("Game ended. Winner: %s", isPlayer1Win ? "Player 1" : "Player 2");
}