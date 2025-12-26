#include "BattleScene.h"
#include "Battlefield.h"
#include "TowerBase.h"
#include "Card.h"
#include "ManaSystem.h"
#include "BattleManager.h"
#include "CardManager.h"
#include "ManaBar.h"
#include "DataManager.h"
/*
PS:这个头文件是用于DataManager测试的，测试时去掉注释即可
#include <fstream>
#include <string>
*/

USING_NS_CC;

Scene* BattleScene::createScene()
{
    return BattleScene::create();
}

bool BattleScene::init()
{
    if (!Scene::init())
        return false;
    /*
    此为测试程序，使用时会输出一份日志文件来检测DataManager能否正常运行。
    testDataManager();
    */
    // 初始化游戏状态
    _gameStarted = false;
    _gameEnded = false;
    _gameTime = 0.0f;
    _totalGameTime = 10.0f;
    _playerCrowns[0] = _playerCrowns[1] = 0;
    _isPlayer1 = true;
    _selectedCard = nullptr;
    _cardGhost = nullptr;
    _deployDrawNode = nullptr;
    _changeGameMode = false;
    // 创建战场
    _battlefield = Battlefield::create();
    _battlefield->setupBattlefield(1);
    addChild(_battlefield, 0);

    // 设置UI
    setupUI();

    // 设置输入
    setupInput();

    // 初始化管理器
    BattleManager::getInstance()->init(_battlefield);

    // 初始化圣水系统
    ManaSystem::getInstance()->init(5.0f, 10.0f, 0.3333f);
    ManaSystem::getEnemyInstance()->init(5.0f, 10.0f, 0.3333f);
    // 初始化卡牌管理器
    CardManager::getInstance()->init();

    //初始化AI
    _enemyAi = EnemyAISystem::getInstance();
    _enemyAi->init();
    _enemyAi->setBattlefield(_battlefield);
    _enemyAi->setBattleManager(BattleManager::getInstance());
    _enemyAi->startAI();

    // 初始化手牌显示
    initHandCards();
    //初始化next卡牌显示
    initNextCard();
    // 开始游戏
    startGame();

    // 开启更新
    scheduleUpdate();

    return true;
}

void BattleScene::initNextCard()
{


    auto slot = LayerColor::create(Color4B(86, 59, 41, 255), 90, 120);

    slot->setPosition(Vec2(50, 40));
    addChild(slot, 50);

    refreshNextCard();
}


void BattleScene::refreshNextCard()
{
    Card* nextCard = CardManager::getInstance()->getNextCard();

    if (!nextCard)
        return;

    // 1️⃣ 如果还没加到场景
    if (!nextCard->getParent())
    {
        addChild(nextCard, 100); // 比手牌略低
    }
    nextCard->setScale(0.75);
    // 2️⃣ Next 卡固定位置（例子）
    nextCard->setPosition(Vec2(50, 40));
    nextCard->setVisible(true);

    // 4️⃣ 刷新 UI（图片、名字、圣水）
    nextCard->refreshView();
}


void BattleScene::initHandCards()
{
    auto cards = CardManager::getInstance()->getHandCards();
    for (auto card : cards)
    {
        this->addChild(card, 200);
        card->setVisible(false);   // 先全部隐藏
    }

    // 只画 4 个槽位框
    for (int i = 0; i < 4; i++)
    {
        auto slot = LayerColor::create(Color4B(86, 59, 41, 255), 120, 160);
        slot->setPosition(Vec2(250 + 150 * i, 60));
        addChild(slot, 50);

    }

    refreshHandLayout();
}

void BattleScene::refreshHandLayout()
{
    auto handCards = CardManager::getInstance()->getHandCards();
    for (int i = 0; i < handCards.size(); i++)
    {

        auto card = handCards[i];
        if (!card)
        {
            continue;
        }
        if (!card->getParent())
        {
            addChild(card, 100);
        }

        card->setPosition(250 + 150 * i, 60);
        card->setVisible(true);

        card->refreshView();
    }
}

//滑动补牌动画
void BattleScene::onCardUsed(Card* card)
{

    _handLocked = true;
    Card* nextCard = CardManager::getInstance()->getNextCard();
    if (!nextCard) return;

    Vec2 fromPos = nextCard->getPosition(); // next 卡槽
    Vec2 toPos = card->getPosition();     // 空槽位
    CCLOG("===onCardUsed===");
    nextCard->setPosition(fromPos);
    nextCard->setVisible(true);

    nextCard->runAction(Sequence::create(
        Spawn::create(
            MoveTo::create(0.5f, toPos),
            ScaleTo::create(0.5f, 1.0f),
            nullptr
        ),
        CallFunc::create([this]() {
            refreshHandLayout();
            refreshNextCard();
            _handLocked = false;
            }),
        nullptr
    ));
}



void BattleScene::setupUI()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // 创建圣水条
    _manaBar = ManaBar::create();
    _manaBar->setPosition(visibleSize.width / 4, 0);
    addChild(_manaBar, 100);
    //_manaBar->setManaValue(5, 10, false);

    // 创建倒计时标签
    auto countdownLabel = Label::create("Time Left :", "fonts/Clash_Regular.otf", 20);
    auto countdown = Label::createWithSystemFont("3:00", "fonts/Clash_Regular.otf", 48);
    countdown->setPosition(visibleSize.width - 80, visibleSize.height - 70);
    countdownLabel->setPosition(visibleSize.width - 80, visibleSize.height - 35);
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
        Color4F(0, 0, 0, 0.5f)  // 50%黑色填充
    );

    // 绘制边框
    drawNode->drawRect(
        Vec2(visibleSize.width - 140, visibleSize.height - 95),           // 左下角
        Vec2(visibleSize.width - 20, visibleSize.height - 20),           // 右上角
        Color4F(0, 0, 0, 1.0f)    // 黑色边框
    );

    addChild(drawNode, 90);

    //绘制圣水倍率"X1"
    auto closeLabel = Label::createWithSystemFont("\\", "Arial Bold", 48);
    auto closeLabe2 = Label::createWithSystemFont("/", "Arial Bold", 48);
    closeLabel->setTextColor(Color4B(128, 0, 128, 255));
    closeLabel->setPosition(visibleSize.width - 90, visibleSize.height - 120);
    addChild(closeLabel, 100);
    closeLabe2->setTextColor(Color4B(128, 0, 128, 255));
    closeLabe2->setPosition(visibleSize.width - 90, visibleSize.height - 120);
    addChild(closeLabe2, 100);

    auto _manaMutiple = Label::createWithSystemFont("1", "Arial Bold", 48);
    _manaMutiple->setTextColor(Color4B(128, 0, 128, 255));
    _manaMutiple->setPosition(visibleSize.width - 60, visibleSize.height - 120);
    addChild(_manaMutiple, 100);
    _manaMutiple->setTag(20);
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
    if (_handLocked)
        return false;

    if (!_gameStarted || _gameEnded)
        return false;

    Vec2 touchPos = touch->getLocation();

    // 1️ 通过 CardManager，判断是否点中了某张手牌
    _selectedCard = CardManager::getInstance()->getCardAtWorldPos(touchPos);

    // 没点中卡牌，直接返回，不吃触摸
    if (!_selectedCard)
        return false;

    // 设置选中状态（放大 + 高亮）
    _selectedCard->setSelected(true);

    // 提升层级，保证拖拽时在最上面
    _selectedCard->setLocalZOrder(1000);

    // 创建拖拽虚影（只创建一次）
    if (!_cardGhost)
    {
        _cardGhost = Sprite::create();
        auto color = LayerColor::create(Color4B(255, 255, 255, 120), 120, 160);
        _cardGhost->addChild(color);
        addChild(_cardGhost, 1500); // 比卡牌还高
    }

    _cardGhost->setPosition(touchPos);
    _cardGhost->setVisible(true);

    return true; // 吃掉这次触摸
}

void BattleScene::onTouchMoved(Touch* touch, Event* event)
{
    // 没有选中卡牌 / 没有虚影，直接返回
    if (!_selectedCard || !_cardGhost)
        return;

    showDeployPosition();
    // 当前触摸的世界坐标
    Vec2 touchPos = touch->getLocation();

    // 虚影跟随鼠标
    _cardGhost->setPosition(touchPos);

    // 判断整体是否为可部署区域
    bool isValid = _battlefield->isValidDeployPosition(
        touchPos,
        _isPlayer1 ? 1 : 2
    );

    // 根据是否合法，修改“卡牌虚影”的颜色
    if (!_cardGhost->getChildren().empty())
    {
        auto layerColor =
            dynamic_cast<LayerColor*>(_cardGhost->getChildren().at(0));

        if (layerColor)
        {
            if (isValid)
            {
                layerColor->setColor(Color3B(100, 255, 100)); // 绿色
                layerColor->setOpacity(150);
            }
            else
            {
                layerColor->setColor(Color3B(255, 100, 100)); // 红色
                layerColor->setOpacity(100);
            }
        }
    }

    // 高亮当前鼠标所在的“格子”
    int row = -1;
    int col = -1;

    if (_battlefield->worldToGrid(touchPos, row, col))
    {
        // 在地图格子范围内 → 高亮该格子
        _battlefield->highlightGrid(row, col, isValid);
    }
    else
    {
        // 不在地图内 → 清除高亮
        _battlefield->clearHighlight();
    }
}


void BattleScene::onTouchEnded(Touch* touch, Event* event)
{
    if (_cardGhost && _selectedCard)
    {
        Vec2 touchPos = touch->getLocation();

        // 检查是否可以部署
        if (_battlefield->isValidDeployPosition(touchPos, _isPlayer1 ? 1 : 2))
        {
            // 检查圣水是否足够
            float manaCost = _selectedCard->getManaCost();
            auto manaSystem = ManaSystem::getInstance();

            if (manaSystem->hasEnoughMana(manaCost))
            {
                if (manaSystem->consumeMana(manaCost))
                {
                    // 使用卡牌
                    if (_selectedCard->use(touchPos, _isPlayer1 ? 1 : 2))
                    {
                        // 从手牌中移除该卡牌
                        auto cardManager = CardManager::getInstance();
                        cardManager->useCard(_selectedCard);
                        onCardUsed(_selectedCard);
                        CCLOG("Deployed card at (%.1f, %.1f)", touchPos.x, touchPos.y);
                    }
                }
            }
        }

        // 取消选择
        _selectedCard->setSelected(false);
        _selectedCard = nullptr;
        _cardGhost->setVisible(false);
    }
    clearDeployPosition();
    _battlefield->clearHighlight();
    refreshHandLayout();
}

void BattleScene::showDeployPosition()
{
    std::vector<Area>unDeployarea = _battlefield->getUnDeployarea();
    // 检查是否已经创建了 DrawNode，如果没有则创建
    if (!_deployDrawNode)
    {
        _deployDrawNode = cocos2d::DrawNode::create();
        this->addChild(_deployDrawNode, 2000); // 只创建一次
    }

    // 清空之前的绘制内容
    _deployDrawNode->clear();

    for (auto& area : unDeployarea)
    {
        // 绘制透明填充矩形（浅红色）
        _deployDrawNode->drawSolidRect(
            Vec2(80 + area.leftBottom.x * _battlefield->getGridSize().width, 280 + area.leftBottom.y * _battlefield->getGridSize().height),
            Vec2(80 + (area.rightTop.x + 1) * _battlefield->getGridSize().width, 280 + (area.rightTop.y + 1) * _battlefield->getGridSize().height),
            Color4F(1.0f, 0.0f, 0.0f, 0.4f)  // 40%透明红色
        );
    }
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

    if (minutes >= 0 && minutes < 1)
    {
        auto _mana = dynamic_cast<Label*>(this->getChildByTag(20));
        if (_mana)
        {
            if (_changeGameMode == false)
            {
                _mana->setString(StringUtils::format("%d", 2));
            }
            else
            {
                _mana->setString(StringUtils::format("%d", 3));
            }
        }
    }
    auto countdownLabel = dynamic_cast<Label*>(getChildByTag(1001));
    if (countdownLabel)
    {
        countdownLabel->setString(StringUtils::format("%2d:%02d", minutes, seconds));
    }

    // 更新圣水系统
    ManaSystem::getInstance()->update(delta);
    ManaSystem::getEnemyInstance()->update(delta);
    // 更新战斗管理器
    BattleManager::getInstance()->update(delta);

    // 更新卡牌管理器
    CardManager::getInstance()->update(delta);
    CardManager::getEnemyInstance()->update(delta);
    //更新AI
    EnemyAISystem::getInstance()->update(delta);
    //改变模式后更新倒计时
    if (BattleManager::getInstance()->getCurrentGameMode() == "sudden_death" && _changeGameMode == false)
    {
        _gameTime = 0.0f;
        _changeGameMode = true;
    }
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

void BattleScene::clearDeployPosition()
{
    if (_deployDrawNode)
    {
        _deployDrawNode->clear();
    }
}


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

/*测试DataManager, 由于容易编码错误，log日志生成到项目根目录（需要请自行去掉注释）
void BattleScene::testDataManager() {
    auto dataMgr = DataManager::getInstance();
    std::string logFilePath = "D:/teamwork/Clash-Royale/test_log.txt";//这里是项目根目录
    std::ofstream logFile(logFilePath, std::ios::out | std::ios::trunc); // 清空原有内容，重新写入
    // 定义日志内容（和输出窗口的日志一致）
    std::string logContent;
    logContent += "==================================\n";
    logContent += "========= DataManager Test =======\n";
    logContent += "==================================\n";
    // 1. 卡牌总数
    int cardCount = dataMgr->getCardCount();
    char buffer[256];
    sprintf(buffer, "Card Total Number: %d\n\n", cardCount);
    logContent += buffer;
    log(buffer);
    // 2. 检查ID=1的卡牌
    logContent += "--- Check Card ID=1 ---\n";
    log("--- Check Card ID=1 ---");
    ValueMap card1 = dataMgr->getCardDataById(1);
    if (!card1.empty()) {
        sprintf(buffer, "ID: %d\n", card1["id"].asInt());
        logContent += buffer; log(buffer);

        sprintf(buffer, "Name: %s\n", card1["name"].asString().c_str());
        logContent += buffer; log(buffer);

        sprintf(buffer, "Mana Cost: %.0f\n", card1["manaCost"].asFloat());
        logContent += buffer; log(buffer);

        sprintf(buffer, "Image Path: %s\n", card1["imgPath"].asString().c_str());
        logContent += buffer; log(buffer);
    }
    else {
        logContent += "Card ID=1 NOT FOUND!\n";
        log("Card ID=1 NOT FOUND!");
    }
    // 3. 所有卡牌列表
    logContent += "\n--- All Cards List ---\n";
    log("\n--- All Cards List ---");
    ValueVector allCards = dataMgr->getAllCardData();
    for (int i = 0; i < allCards.size(); i++) {
        ValueMap card = allCards[i].asValueMap();
        sprintf(buffer, "Card %d: ID=%d, Name=%s, Mana=%.0f\n",
            i + 1,
            card["id"].asInt(),
            card["name"].asString().c_str(),
            card["manaCost"].asFloat());
        logContent += buffer; log(buffer);
    }
    logContent += "==================================\n";
    logContent += "========= Test Finished ==========\n";
    logContent += "==================================\n";
    //把日志内容写入本地文件
    if (logFile.is_open()) {
        logFile.write(logContent.c_str(), logContent.size());
        logFile.close();
        log("日志已写入本地文件：test_log.txt");
    }
    else {
        log("无法创建日志文件！");
    }
}
*/