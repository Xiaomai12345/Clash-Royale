#include "Battlefield.h"
#include"KnightTroop.h"
#include"ArcherTroop.h"
#include"TroopAIComponent.h"
#include"SimpleTroopAIComponent.h"
#include"PrincessTower.h"
#include"FireBallSpell.h"
#include"SlowDownSpell.h"
USING_NS_CC;

bool Battlefield::init()//ch
{
    if (!Node::init())
        return false;

    _debugEnabled = false;
    _debugDrawNode = nullptr;

    myArea1=Area(0, 11, 8, 14);
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

    auto troop1 = KnightTroop::create();
    troop1->setPosition(Vec2(300, 300));
    this->addChild(troop1);

    auto troop2 = KnightTroop::create();
    troop2->setPosition(Vec2(340, 300));
    this->addChild(troop2);

    auto troop3 = KnightTroop::create();
    troop3->setPosition(Vec2(500, 300)); // 故意放远
    this->addChild(troop3);

    auto building = PrincessTower::create();
    building->setPosition(Vec2(300, 1024));
    building->setCamp(ECamp::RIGHT);
    this->addChild(building);

    auto building1 = PrincessTower::create();
    building1->setPosition(Vec2(500, 1024));
    building1->setCamp(ECamp::RIGHT);
    this->addChild(building1);


    auto fireball = SlowDownSpell::create();
    this->addChild(fireball);
    fireball->cast(Vec2(320, 300), ECamp::RIGHT); // 固定测试点

    if (_debugEnabled)
        createDebugLayer();
    CCLOG("Setup battlefield level %d", level);

}

void Battlefield::createBackground()
{
    // 尝试加载图片背景
    _background = Sprite::create("Images/background/battlefield.jpg");
     CCLOG("Successfully loaded battlefield background image");
     // 获取原始尺寸并计算缩放
     Size originalSize = _background->getContentSize();
     float scaleX = 0.8 * 0.85;
     float scaleY = 0.8 * 0.8;

     // 设置缩放
     _background->setScaleX(scaleX);
     _background->setScaleY(scaleY);


     CCLOG("Scaled size: %.0f x %.0f",
         originalSize.width * scaleX,
         originalSize.height * scaleY);

    _background->setAnchorPoint(Vec2(0.5f, 1.0f));  // 锚点在顶部中间
    _background->setPosition(_mapSize.width * 0.8 * 0.85 / 2, _mapSize.height * 0.8-50);  // 放在顶部
    addChild(_background, 0);
    auto lavenderRect = LayerColor::create(Color4B(82, 150, 111, 255), _mapSize.width * 0.8 * 0.85, 50);
    lavenderRect->setPosition(0, _mapSize.height * 0.8-50);
    this->addChild(lavenderRect);

    auto drawNode = cocos2d::DrawNode::create();
    this->addChild(drawNode,2000);

    // 绘制矩形，位置是世界坐标，宽和高是矩形的尺寸
    drawNode->drawRect(Vec2(80, 280) , Vec2(20*gridWeight, 1732 * 0.8 - 110), Color4F::RED);
}

bool Battlefield::worldToGrid(const Vec2& worldPos, int& outRow, int& outCol) const
{
    outCol = static_cast<int>((worldPos.x-80) / _gridSize.width);
    outRow = static_cast<int>((worldPos.y-280) / _gridSize.height);

    if (outRow < 0 || outRow >= _rows || outCol < 0 || outCol >= _cols)
        return false;

    return true;
}

Vec2 Battlefield::gridToWorld(int row, int col) const
{
    return Vec2(
        col * _gridSize.width + _gridSize.width * 0.5f,
        row * _gridSize.height + _gridSize.height * 0.5f
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
                c * _gridSize.width + _gridSize.width * 0.5f,
                r * _gridSize.height + _gridSize.height * 0.5f
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

    if (playerId == 0)
        return grid->getRegionType() == Grid::RegionType::PLAYER;

    if (playerId == 1)
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