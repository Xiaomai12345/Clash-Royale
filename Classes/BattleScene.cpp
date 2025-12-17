#include "BattleScene.h"
#include "Battlefield.h"
#include "TowerBase.h"
#include "Card.h"
#include "ManaSystem.h"
#include "BattleManager.h"
#include "CardManager.h"
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

    // 初始化管理器
    BattleManager::getInstance()->init(_battlefield);

    // 初始化圣水系统
    ManaSystem::getInstance()->init(5.0f, 10.0f, 0.8f);

    // 初始化卡牌管理器
    CardManager::getInstance()->init();

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
    auto countdownLabel = Label::createWithSystemFont("03:00", "Arial", 36);
    countdownLabel->setPosition(visibleSize.width / 2, visibleSize.height - 100);
    countdownLabel->setTextColor(Color4B::WHITE);
    countdownLabel->setTag(1001);
    addChild(countdownLabel, 100);

    // 创建皇冠计数器
    auto createCrownDisplay = [this](int playerId, Vec2 position) {
        auto node = Node::create();

        // 皇冠图标
        auto crown = Sprite::create();
        auto crownColor = LayerColor::create(Color4B(255, 215, 0, 255), 30, 30);
        crown->addChild(crownColor);
        crown->setScale(0.8f);
        node->addChild(crown, 0);

        // 数量标签
        auto label = Label::createWithSystemFont("0", "Arial", 24);
        label->setPosition(25, 0);
        label->setTextColor(Color4B::WHITE);
        label->setTag(10);
        node->addChild(label, 1);

        node->setPosition(position);
        node->setTag(2000 + playerId);
        addChild(node, 100);
        };

    createCrownDisplay(1, Vec2(100, visibleSize.height - 50));
    createCrownDisplay(2, Vec2(visibleSize.width - 100, visibleSize.height - 50));

    // 调试按钮
    auto debugButton = MenuItemLabel::create(
        Label::createWithSystemFont("Debug", "Arial", 24),
        [this](Ref* sender) {
            _battlefield->enableDebugDraw(!_battlefield->isDebugDrawEnabled());
        }
    );

    debugButton->setPosition(Vec2(visibleSize.width - 80, 50));

    auto menu = Menu::create(debugButton, nullptr);
    menu->setPosition(Vec2::ZERO);
    addChild(menu, 200);
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

    // 检查是否点击了卡牌（简化版本）
    auto cardManager = CardManager::getInstance();
    auto handCards = cardManager->getHandCards();

    // 如果手牌区域有卡牌，可以选择（简化逻辑）
    if (!handCards.empty())
    {
        _selectedCard = handCards[0]; // 选择第一张卡牌作为示例
        _selectedCard->setSelected(true);
    }

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

void BattleScene::onTouchMoved(Touch* touch, Event* event)
{
    if (_cardGhost)
    {
        Vec2 touchPos = touch->getLocation();
        _cardGhost->setPosition(touchPos);

        // 检查部署位置是否有效
        bool isValid = _battlefield->isValidDeployPosition(touchPos, _isPlayer1 ? 1 : 2);

        // 改变虚影颜色表示有效性
        auto layerColor = dynamic_cast<LayerColor*>(_cardGhost->getChildren().at(0));
        if (layerColor)
        {
            if (isValid)
            {
                layerColor->setColor(Color3B(100, 255, 100));
                layerColor->setOpacity(150);
            }
            else
            {
                layerColor->setColor(Color3B(255, 100, 100));
                layerColor->setOpacity(100);
            }
        }
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
                        cardManager->useCard(_selectedCard, touchPos, _isPlayer1 ? 1 : 2);

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
}

void BattleScene::startGame()
{
    _gameStarted = true;
    _gameTime = 0.0f;

    CCLOG("Game started!");
}

void BattleScene::update(float delta)
{
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

    auto countdownLabel = dynamic_cast<Label*>(getChildByTag(1001));
    if (countdownLabel)
    {
        countdownLabel->setString(StringUtils::format("%02d:%02d", minutes, seconds));
    }

    // 检查游戏结束条件
    if (remainingTime <= 0)
    {
        endGame(_playerCrowns[0] > _playerCrowns[1]);
        return;
    }

    // 更新圣水系统
    ManaSystem::getInstance()->update(delta);

    // 更新战斗管理器
    BattleManager::getInstance()->update(delta);

    // 更新卡牌管理器
    CardManager::getInstance()->update(delta);

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