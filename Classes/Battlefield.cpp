#include "Battlefield.h"
#include"TroopAIComponent.h"
#include"SimpleTroopAIComponent.h"
#include"BattleManager.h"
#include"AllCards.h"
#include"CardFactory.h"
#include "EnemyAISystem.h"
USING_NS_CC;

bool Battlefield::init()//ch
{
    if (!Node::init())
        return false;

    _debugEnabled = false;
    _debugDrawNode = nullptr;

    myArea1 = Area(0, 11, 8, 14);
    myArea2 = Area(9, 11, 17, 14);
    myArea3 = Area(0, 0, 17, 10);

    enemyArea1 = Area(0, 17, 8, 20);
    enemyArea2 = Area(9, 17, 17, 20);
    enemyArea3 = Area(0, 21, 17, 31);

    bridgeArea1 = Area(3, 15, 3, 16);
    bridgeArea2 = Area(14, 15, 14, 16);

    riverArea1 = Area(0, 15, 2, 16);
    riverArea2 = Area(4, 15, 8, 16);
    riverArea3 = Area(9, 15, 13, 16);
    riverArea4 = Area(15, 15, 17, 16);

    myArea.push_back(myArea1);
    myArea.push_back(myArea2);
    myArea.push_back(myArea3);

    enemyArea.push_back(enemyArea1);
    enemyArea.push_back(enemyArea2);
    enemyArea.push_back(enemyArea3);

    bridgeArea.push_back(bridgeArea1);
    bridgeArea.push_back(bridgeArea2);

    riverArea.push_back(riverArea1);
    riverArea.push_back(riverArea2);
    riverArea.push_back(riverArea3);
    riverArea.push_back(riverArea4);

    /*setContentSize(_mapSize);*/

    return true;
}

void Battlefield::setupBattlefield(int level)
{
    _mapSize = Size(1308, 1732);
    _gridSize = Size(gridWeight, gridHeight);


    createBackground();
    createGrids();
    buildRegions();

    BattleManager::getInstance()->init(/* battlefield 指针 */ this);
    setupGameEndCallback();
    auto EnemyLeft = PrincessTower::create();
    EnemyLeft->setPosition(Vec2(300, 1024));
    EnemyLeft->setCamp(ECamp::RIGHT);
    this->addChild(EnemyLeft);

 
    auto EnemyRight = PrincessTower::create();
    EnemyRight->setPosition(Vec2(500, 1024));
    EnemyRight->setCamp(ECamp::RIGHT);
    this->addChild(EnemyRight);

    auto EnemyKing = KingdomTower::create();
    EnemyKing->setPosition(Vec2(450, 1200));
    EnemyKing->setCamp(ECamp::RIGHT);
   
    this->addChild(EnemyKing);


    auto MyLeft = PrincessTower::create();
    MyLeft->setPosition(Vec2(200, 500));
    MyLeft->setCamp(ECamp::LEFT);
    this->addChild(MyLeft);

    auto MyRight = PrincessTower::create();
    MyRight->setPosition(Vec2(650, 500));
    MyRight->setCamp(ECamp::LEFT);
    this->addChild(MyRight);

    auto MyKing = KingdomTower::create();
    MyKing->setPosition(Vec2(450, 300));
    MyKing->setCamp(ECamp::LEFT);
    this->addChild(MyKing);



    if (_debugEnabled)
        createDebugLayer();
    
  
    CCLOG("Setup battlefield level %d", level);

}

void Battlefield::createBackground()
{
    // 创建背景图片
    _background = Sprite::create("Images/background/battlefield.jpg");
    CCLOG("Successfully loaded battlefield background image");
    // 设置缩放
    Size originalSize = _background->getContentSize();
    float scaleX = 0.8 * 0.85;
    float scaleY = 0.8 * 0.8;

    _background->setScaleX(scaleX);
    _background->setScaleY(scaleY);


    CCLOG("Scaled size: %.0f x %.0f",
        originalSize.width * scaleX,
        originalSize.height * scaleY);

    _background->setAnchorPoint(Vec2(0.5f, 1.0f));  // 设置锚点在顶部中间
    _background->setPosition(_mapSize.width * 0.8 * 0.85 / 2, _mapSize.height * 0.8 - 50);  
    addChild(_background, 0);
    auto lavenderRect = LayerColor::create(Color4B(82, 150, 111, 255), _mapSize.width * 0.8 * 0.85, 50);
    lavenderRect->setPosition(0, _mapSize.height * 0.8 - 50);
    this->addChild(lavenderRect);

    auto slot = LayerColor::create(Color4B(142,111,84,255), _mapSize.width * 0.8 * 0.85, 229);
    slot->setPosition(0, 0);
    this->addChild(slot);
    //auto drawNode = cocos2d::DrawNode::create();
    //this->addChild(drawNode, 2000);

    //// 生成矩形
    //drawNode->drawRect(Vec2(80, 280), Vec2(20 * gridWeight, 1732 * 0.8 - 110), Color4F::RED);
}

bool Battlefield::worldToGrid(const Vec2& worldPos, int& outRow, int& outCol) const
{
    outCol = static_cast<int>((worldPos.x - 80) / _gridSize.width);
    outRow = static_cast<int>((worldPos.y - 280) / _gridSize.height);

    if (outRow < 0 || outRow >= _rows || outCol < 0 || outCol >= _cols)
        return false;

    return true;
}

Vec2 Battlefield::gridToWorld(int row, int col) const
{
    return Vec2(
        col * _gridSize.width + _gridSize.width * 0.5f + 80,
        row * _gridSize.height + _gridSize.height * 0.5f + 280
    );
}

Grid* Battlefield::getGrid(int row, int col)
{
    return &_grids[row * _cols + col];
}

const Grid* Battlefield::getGrid(int row, int col) const
{
    return &_grids[row * _cols + col];
}

void Battlefield::buildRegions()
{
    for (auto& grid : _grids)
    {
        int y = grid.getRow();
        int x = grid.getCol();
        grid.setRegionType(Grid::RegionType::NONE);
        grid.setWalkable(true);
        grid.setDeployable(false);

        for (auto& area : myArea)
        {
            if (x >= area.leftBottom.x && y >= area.leftBottom.y && x <= area.rightTop.x && y <= area.rightTop.y)
            {
                grid.setRegionType(Grid::RegionType::PLAYER);
                grid.setDeployable(true);
                grid.setWalkable(true);
                break;
            }
        }
        for (auto& area : enemyArea)
        {
            if (x >= area.leftBottom.x && y >= area.leftBottom.y && x <= area.rightTop.x && y <= area.rightTop.y)
            {
                grid.setRegionType(Grid::RegionType::ENEMY);
                grid.setDeployable(true);
                grid.setWalkable(true);
                break;
            }
        }
        for (auto& area : bridgeArea)
        {
            if (x >= area.leftBottom.x && y >= area.leftBottom.y && x <= area.rightTop.x && y <= area.rightTop.y)
            {
                grid.setRegionType(Grid::RegionType::BRIDGE);
                grid.setDeployable(true);
                grid.setWalkable(true);
                break;
            }
        }
        for (auto& area : riverArea)
        {
            if (x >= area.leftBottom.x && y >= area.leftBottom.y && x <= area.rightTop.x && y <= area.rightTop.y)
            {
                grid.setRegionType(Grid::RegionType::RIVER);
                grid.setDeployable(false);
                grid.setWalkable(false);
                break;
            }
        }
    }
}

void Battlefield::createGrids()
{
    _grids.clear();
    _grids.reserve(_rows * _cols);

    for (int r = 0; r < _rows; ++r)
    {
        for (int c = 0; c < _cols; ++c)
        {
            Vec2 worldPos(
                c * _gridSize.width + _gridSize.width * 0.5f + 80,
                r * _gridSize.height + _gridSize.height * 0.5f + 280
            );
            _grids.emplace_back(r, c, worldPos);
        }
    }
}

void Battlefield::createDebugLayer()
{
    if (_debugDrawNode)
        _debugDrawNode->removeFromParent();

    _debugDrawNode = DrawNode::create();
    addChild(_debugDrawNode, 100);

    drawDebugGrids();
}

bool Battlefield::isValidDeployPosition(const Vec2& worldPos, int playerId) const
{
    int r, c;
    if (!worldToGrid(worldPos, r, c))
        return false;

    const Grid* grid = getGrid(r, c);
    if (!grid->isDeployable())
        return false;

    if (playerId == 1)
    {
        for (auto& area : myArea)
        {
            if (grid->getCol() >= area.leftBottom.x && grid->getRow() >= area.leftBottom.y
                && grid->getCol() <= area.rightTop.x && grid->getRow() <= area.rightTop.y)
            {
                return true;
            }
        }
    }

    if (playerId == 2)
    {
        for (auto& area : enemyArea)
        {
            if (grid->getCol() >= area.leftBottom.x && grid->getRow() >= area.leftBottom.y
                && grid->getCol() <= area.rightTop.x && grid->getRow() <= area.rightTop.y)
            {
                return true;
            }
        }
    }

    return false;
}

bool Battlefield::isWalkable(const Vec2& worldPos) const
{
    int r, c;
    if (!worldToGrid(worldPos, r, c))
        return false;

    return getGrid(r, c)->isWalkable();
}

void Battlefield::enableDebugDraw(bool enable)
{
    _debugEnabled = enable;

    if (enable)
        createDebugLayer();
    else if (_debugDrawNode)
        _debugDrawNode->removeFromParent();
}
void Battlefield::drawDebugGrids()
{
    for (const auto& grid : _grids)
    {
        Color4F color = Color4F::WHITE;

        switch (grid.getRegionType())
        {
        case Grid::RegionType::PLAYER: color = Color4F::GREEN; break;
        case Grid::RegionType::ENEMY:  color = Color4F::RED; break;
        case Grid::RegionType::RIVER:  color = Color4F::BLUE; break;
        case Grid::RegionType::BRIDGE: color = Color4F::YELLOW; break;
        default: break;
        }

        Vec2 center = grid.getWorldPos();
        Vec2 half(_gridSize.width / 2, _gridSize.height / 2);

        _debugDrawNode->drawRect(center - half, center + half, color);
    }
}
void Battlefield::addUnit(cocos2d::Node* unit)
{
    if (unit)
    {
        this->addChild(unit); // 将单位添加到场景中
        CCLOG("Unit added to battlefield");
    }
}
bool Battlefield::canWalk(const Vec2& worldPos) const
{
    int r, c;
    if (!worldToGrid(worldPos, r, c))
        return false;

    return getGrid(r, c)->isWalkable();
}
bool Battlefield::isBridge(const cocos2d::Vec2& worldPos) const
{
    int r, c;
    if (!worldToGrid(worldPos, r, c))
        return false;

    const Grid* grid = getGrid(r, c);
    if (!grid)
        return false;

    return grid->getRegionType() == Grid::RegionType::BRIDGE;
}
Vec2 Battlefield::constrainPosition(const Vec2& desired, const Vec2& current) const
{
    // 1. 完全不可走（比如河）
    if (!canWalk(desired))
    {
        // 不允许进入，直接退回
        return current;
    }

    // 2. 在桥上 → 通道投影
    if (isBridge(desired))
    {
        return projectToBridge(desired);
    }

    // 3. 普通地面
    return desired;
}
Vec2 Battlefield::projectToBridge(const Vec2& desiredPos) const
{
    const Area* bridge = findBridgeArea(desiredPos);
    if (!bridge)
        return desiredPos;

    // 1. 计算桥的中心列（Grid）
    float centerCol = (bridge->leftBottom.x + bridge->rightTop.x) * 0.5f;

    // 2. 转成世界坐标
    Vec2 centerWorld = gridToWorld(0, centerCol);
    float bridgeCenterX = centerWorld.x;

    // 3. 计算桥宽（用 grid 宽度）
    float bridgeGridWidth =
        bridge->rightTop.x - bridge->leftBottom.x + 1;

    float halfWidth = (bridgeGridWidth * gridWeight) * 0.5f;

    // 4. Clamp X（投影）
    float clampedX = clampf(
        desiredPos.x,
        bridgeCenterX - halfWidth,
        bridgeCenterX + halfWidth
    );

    return Vec2(clampedX, desiredPos.y);
}
const Area* Battlefield::findBridgeArea(const Vec2& worldPos) const
{
    int row, col;
    if (!worldToGrid(worldPos, row, col))
        return nullptr;

    for (const Area& area : bridgeArea)
    {
        if (col >= area.leftBottom.x && col <= area.rightTop.x &&
            row >= area.leftBottom.y && row <= area.rightTop.y)
        {
            return &area;
        }
    }
    return nullptr;
}

float Battlefield::getNearestBridgeX(const cocos2d::Vec2& currentPos) const
{
    float minDist = FLT_MAX;
    float targetX = currentPos.x;

    for (const auto& area : bridgeArea)
    {
        // 计算桥中心X
        float centerCol = (area.leftBottom.x + area.rightTop.x) * 0.5f;
        Vec2 centerWorld = gridToWorld(0, centerCol); // row无关，取X
        float bridgeX = centerWorld.x;

        float dist = std::abs(currentPos.x - bridgeX);
        if (dist < minDist)
        {
            minDist = dist;
            targetX = bridgeX;
        }
    }
    return targetX;
}


float Battlefield::getNearestBridgeY(const cocos2d::Vec2& currentPos) const
{
    float minDist = FLT_MAX;
    float targetY = currentPos.y;

    for (const auto& area : bridgeArea)
    {
        // 1. 桥的下边界 Y（grid → world）
        Vec2 bottomWorld = gridToWorld(area.leftBottom.y, 0);
        float bottomY = bottomWorld.y;

        // 2. 桥的上边界 Y（grid → world）
        Vec2 topWorld = gridToWorld(area.rightTop.y, 0);
        float topY = topWorld.y;

        // 3. 比较当前位置离上下边界哪个更近
        float distToBottom = std::abs(currentPos.y - bottomY);
        float distToTop = std::abs(currentPos.y - topY);

        // 4. 选择更近的那个
        if (distToBottom < minDist)
        {
            minDist = distToBottom;
            targetY = bottomY;
        }

        if (distToTop < minDist)
        {
            minDist = distToTop;
            targetY = topY;
        }
    }

    return targetY;
}

bool Battlefield::hasRiverBetween(const cocos2d::Vec2& p1, const cocos2d::Vec2& p2) const
{
    int r1, c1, r2, c2;
    if (!worldToGrid(p1, r1, c1) || !worldToGrid(p2, r2, c2))
    {
        // 如果有坐标不在网格内（例如飞出地图），保守起见认为没有河（或者直接走直线）
        return false;
    }

    // 河流区域是 row 15 和 16
    // Side A: row <= 14
    // Side B: row >= 17
    
    // 我们定义两个区域：Bottom (row <= 14) 和 Top (row >= 17)
    // 如果一个在 Bottom 一个在 Top，则有河。
    // 如果其中一个在 River (15, 16) 中，我们认为它已经在“桥”或“河”上了，不需要寻桥逻辑介入
    // (因为寻桥逻辑是把人引导到桥口，如果已经在河区域，说明已经在过桥了)
    
    bool p1Bottom = (r1 <= 14);
    bool p1Top    = (r1 >= 17);
    
    bool p2Bottom = (r2 <= 14);
    bool p2Top    = (r2 >= 17);

    // 如果一个在下半区，一个在上半区，说明中间隔着河
    if ((p1Bottom && p2Top) || (p1Top && p2Bottom))
    {
        return true;
    }
    
    return false;
}

std::vector<Area>Battlefield::getUnDeployarea()
{

    std::vector<Area>t{ enemyArea3 };
    if (BattleManager::getInstance()->getEnemyLeftPrincessAlive())
    {
        t.push_back(enemyArea1);
        t.push_back(bridgeArea1);
        t.push_back(riverArea1);
        t.push_back(riverArea2);
    }
    if (BattleManager::getInstance()->getEnemyRightPrincessAlive())
    {
        t.push_back(enemyArea2);
        t.push_back(bridgeArea2);
        t.push_back(riverArea3);
        t.push_back(riverArea4);
    }
    return t;

}
void Battlefield::setupGameEndCallback()
{
    auto battleManager = BattleManager::getInstance();

    // 设置游戏结束回调
    battleManager->setGameEndCallback([this](int winner) {
        // winner: 0=平局, 1=玩家1胜利, 2=玩家2胜利
        bool isPlayer1Win = (winner == 1);
        this->showGameEndUI(isPlayer1Win);
        });
}

void Battlefield::showGameEndUI(bool isPlayer1Win)
{
    // ========== 1. 先创建事件屏蔽层 ==========
    createEventBlocker();
    // =======================================

    auto visibleSize = Director::getInstance()->getVisibleSize();

    // 创建半透明背景层
    auto resultLayer = LayerColor::create(Color4B(0, 0, 0, 180),
        visibleSize.width,
        visibleSize.height);
    resultLayer->setTag(999);  // 设置tag以便移除
    this->addChild(resultLayer, 1000);

    // 结果文本
    std::string resultText;
    cocos2d::Color4B textColor;

    if (isPlayer1Win)
    {
        resultText = "Victory!";
        textColor = Color4B::GREEN;
    }
    else
    {
        resultText = "Defeat!";
        textColor = Color4B::RED;
    }

    auto resultLabel = Label::createWithSystemFont(resultText, "Arial", 72);
    resultLabel->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 100);
    resultLabel->setTextColor(textColor);
    resultLayer->addChild(resultLabel);

    // 获取皇冠数显示
    auto battleManager = BattleManager::getInstance();
    std::string crownText = StringUtils::format("Crowns: %d - %d",
        battleManager->getPlayerCrowns(0),  // 玩家1（我方）
        battleManager->getPlayerCrowns(1)); // 玩家2（敌方）

    auto crownLabel = Label::createWithSystemFont(crownText, "Arial", 36);
    crownLabel->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    crownLabel->setTextColor(Color4B::WHITE);
    resultLayer->addChild(crownLabel);

    // 返回按钮 - 需要允许点击
    auto returnButton = MenuItemLabel::create(
        Label::createWithSystemFont("Return to Menu", "Arial", 32),
        [this](Ref* sender) {
            // 移除事件屏蔽层
            this->removeEventBlocker();

            // 移除结束UI
            this->removeChildByTag(999);

            Director::getInstance()->popScene();
        }
    );
    returnButton->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 100);

    auto menu = Menu::create(returnButton, nullptr);
    menu->setPosition(Vec2::ZERO);
    resultLayer->addChild(menu);

    CCLOG("Game ended. Winner: %s", isPlayer1Win ? "Player 1" : "Player 2");
}
void Battlefield::createEventBlocker()
{
    // 创建一个全屏透明的层来拦截所有触摸事件
    _eventBlocker = Layer::create();
    _eventBlocker->setContentSize(Director::getInstance()->getVisibleSize());
    _eventBlocker->setPosition(Vec2::ZERO);

    // ========== 创建正确的触摸监听器 ==========
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);  // 吞噬触摸事件，不向下传递

    touchListener->onTouchBegan = [](Touch* touch, Event* event) {
        CCLOG("a");
        return true;  // 返回true表示处理了这个触摸，阻止进一步传递
        };

    touchListener->onTouchMoved = [](Touch* touch, Event* event) {
        return true;  // 继续吞噬移动事件
        };

    touchListener->onTouchEnded = [](Touch* touch, Event* event) {
        return true;  // 继续吞噬结束事件
        };

    touchListener->onTouchCancelled = [](Touch* touch, Event* event) {
        return true;  // 继续吞噬取消事件
        };
    // =========================================

    // 注册到事件分发器
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(
        touchListener, _eventBlocker);

    this->addChild(_eventBlocker, 9999);  // 最高层级
    CCLOG("事件屏蔽层已创建");
}

void Battlefield::removeEventBlocker()
{
    if (_eventBlocker)
    {
        // 移除监听器
        if (_touchListener)
        {
            Director::getInstance()->getEventDispatcher()->removeEventListener(_touchListener);
            _touchListener = nullptr;
        }

        _eventBlocker->removeFromParent();
        _eventBlocker = nullptr;
        CCLOG("事件屏蔽层已移除");
    }
}

void Battlefield::highlightGrid(int row, int col, bool valid)
{
    if (!_highlightNode)
    {
        _highlightNode = DrawNode::create();
        addChild(_highlightNode, 999);
    }

    _highlightNode->clear();

    Vec2 lb = gridToWorld(row, col);
    Vec2 rt = lb + getGridSize();
    lb.x = lb.x - _gridSize.width * 0.5f;
    lb.y = lb.y - _gridSize.height * 0.5f;
    rt.x = rt.x - _gridSize.width * 0.5f;
    rt.y = rt.y - _gridSize.height * 0.5f;
    Color4F color = valid
        ? Color4F(0.f, 1.f, 0.f, 0.25f)   // 绿色
        : Color4F(1.f, 0.f, 0.f, 0.25f);  // 红色

    _highlightNode->drawSolidRect(lb, rt, color);
}

void Battlefield::clearHighlight()
{
    if (_highlightNode)
        _highlightNode->clear();
}

void Battlefield::expandMyDeployArea(bool lane)
{
    if (lane == false)
    {
        myArea.push_back(bridgeArea1);
        myArea.push_back(enemyArea1);
    }
    else
    {
        myArea.push_back(bridgeArea2);
        myArea.push_back(enemyArea2);
    }
}
void Battlefield::expandEnemyDeployArea(bool lane)
{
    if (lane == false)
    {
        enemyArea.push_back(bridgeArea1);
        enemyArea.push_back(myArea1);
    }
    else
    {
        enemyArea.push_back(bridgeArea2);
        enemyArea.push_back(myArea2);
    }
}
