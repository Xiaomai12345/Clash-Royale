#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include "cocos2d.h"
#include "Grid.h"
#include "BattleManager.h"
#include <vector>
#include"IWalkableWorld.h"
struct Coordinate
{
    int x;
    int y;
    Coordinate(int x, int y) :x(x), y(y) {}
};
struct Area
{
    Coordinate leftBottom;
    Coordinate rightTop;
    Area(Coordinate leftbottom, Coordinate righttop) :leftBottom(leftbottom), rightTop(righttop) {}
    Area(int x1, int y1, int x2, int y2) :leftBottom(x1, y1), rightTop(x2, y2) {}
    Area() : leftBottom(0, 0), rightTop(0, 0) {}
};
class Battlefield : public cocos2d::Node, public IWalkableWorld
{
public:
    CREATE_FUNC(Battlefield);
    virtual bool init() override;

    void setupBattlefield(int level = 1);

    bool isValidDeployPosition(const cocos2d::Vec2& worldPos, int playerId) const;
    bool isWalkable(const cocos2d::Vec2& worldPos) const;
    bool isBridge(const cocos2d::Vec2& worldPos) const;
    bool canWalk(const cocos2d::Vec2& worldPos) const ;
    cocos2d::Vec2 constrainPosition(const cocos2d::Vec2& desired, const cocos2d::Vec2& current) const;
    const Area* findBridgeArea(const cocos2d::Vec2& worldPos) const;
    cocos2d::Vec2 projectToBridge(const cocos2d::Vec2& desiredPos) const;
    float getNearestBridgeX(const cocos2d::Vec2& currentPos) const override;
    float getNearestBridgeY(const cocos2d::Vec2& currentPos) const override;
    bool worldToGrid(const cocos2d::Vec2& worldPos, int& outRow, int& outCol) const;
    cocos2d::Vec2 gridToWorld(int row, int col) const;

    void enableDebugDraw(bool enable);
    bool isDebugDrawEnabled() const { return _debugEnabled; }
	void addUnit(cocos2d::Node* unit);

    std::vector<Area>getMyarea() { return myArea; }
    std::vector<Area>getEnemyarea() { return enemyArea; }
    std::vector<Area>getDeployarea();
    cocos2d::Size getGridSize() { return _gridSize; }
private:
    void createBackground();
    void createDebugLayer();
    void drawDebugGrids();

    void createGrids();
    void buildRegions();

    Grid* getGrid(int row, int col);
    const Grid* getGrid(int row, int col) const;
    
private:
    cocos2d::Size _mapSize;
    cocos2d::Size _gridSize;

    const float gridHeight = 31.11;
    const float gridWeight = 40.5;
    int _rows = 32;
    int _cols = 18;
    Area myArea1, myArea2, myArea3, enemyArea1, enemyArea2, enemyArea3,
        bridgeArea1, bridgeArea2, riverArea1, riverArea2, riverArea3, riverArea4;
    std::vector<Grid> _grids;
    std::vector<Area>myArea;
    std::vector<Area>enemyArea;
    std::vector<Area>bridgeArea;
    std::vector<Area>riverArea;
    cocos2d::Sprite* _background = nullptr;
    cocos2d::DrawNode* _debugDrawNode = nullptr;

    bool _debugEnabled = false;
};

#endif // BATTLEFIELD_H
