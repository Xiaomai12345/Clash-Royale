#include "Battlefield.h"
#include"KnightTroop.h"
#include"ArcherTroop.h"
#include"TroopAIComponent.h"
#include"SimpleTroopAIComponent.h"
#include"BattleManager.h"
#include"AllCards.h"
#include"CardFactory.h"
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

    // 2️⃣ 创建一张 Knight 卡
    auto card = CardFactory::createKnightCard();
    card->setPosition(Vec2(100, 80));
    this->addChild(card); // 只是为了看见卡，不影响逻辑

    auto card1 = CardFactory::createArcherCard();
    card1->setPosition(Vec2(200, 80));
    this->addChild(card1);

    auto card2 = CardFactory::createGiantCard();
    card2->setPosition(Vec2(300, 80));
    this->addChild(card2);

    auto card3 = CardFactory::createValkyrieCard();
    card3->setPosition(Vec2(400, 80));
    this->addChild(card3);

    auto card4 = CardFactory::createDragonBabyCard();
    card4->setPosition(Vec2(500, 80));
    this->addChild(card4);

    auto card5 = CardFactory::createCannonCard();
    card5->setPosition(Vec2(600, 80)); // 设置位置
    this->addChild(card5);

    auto card6 = CardFactory::createSkeletonCard();
    card6->setPosition(Vec2(700, 80)); // 设置位置
    this->addChild(card6);

    auto card7 = CardFactory::createSkeletonLegionCard();
    card7->setPosition(Vec2(800, 80)); // 设置位置
    this->addChild(card7);

    auto card8 = CardFactory::createSkeletonTombstoneCard();
    this->addChild(card8);


    Vec2 testPos(300, 900);   // 你想让士兵出现的位置
    int playerId = 0;         // 测试用阵营


    card2->use(Vec2(600, 500), !playerId);
    card3->use(Vec2(300, 500), !playerId);
    card2->use(Vec2(700, 500), !playerId);
	card8->use(Vec2(400, 900), playerId);

    auto building = PrincessTower::create();
    building->setPosition(Vec2(300, 1024));
    building->setCamp(ECamp::RIGHT);
    this->addChild(building);

 


    auto troop6 = Giant::create();
    troop6->setPosition(Vec2(100, 300));
    this->addChild(troop6);

    auto troop7 = SkeletonTroop::create();
    troop7->setPosition(Vec2(50, 300));
    this->addChild(troop7);

<<<<<<< Updated upstream

 

    auto tombstone = SkeletonTombstone::create();
    tombstone->setPosition(Vec2(500, 600));  // 
    this->addChild(tombstone);  //



=======
>>>>>>> Stashed changes
    auto building1 = PrincessTower::create();
    building1->setPosition(Vec2(500, 1024));
    building1->setCamp(ECamp::RIGHT);
    this->addChild(building1);

    auto building2 = KingdomTower::create();
    building2->setPosition(Vec2(450, 1200));
    building2->setCamp(ECamp::RIGHT);
    this->addChild(building2);


    auto buildingleft = PrincessTower::create();
    buildingleft->setPosition(Vec2(200, 500));
    buildingleft->setCamp(ECamp::LEFT);
    this->addChild(buildingleft);

    auto building1left = PrincessTower::create();
    building1left->setPosition(Vec2(650, 500));
    building1left->setCamp(ECamp::LEFT);
    this->addChild(building1left);

    auto building2left = KingdomTower::create();
    building2left->setPosition(Vec2(450, 300));
    building2left->setCamp(ECamp::LEFT);
    this->addChild(building2left);



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
                grid.setDeployable(false);
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
        return grid->getRegionType() == Grid::RegionType::PLAYER;

    if (playerId == 2)
        return grid->getRegionType() == Grid::RegionType::ENEMY;

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