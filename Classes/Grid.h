#ifndef GRID_H
#define GRID_H

#include "cocos2d.h"

class Grid
{
public:
    enum class RegionType
    {
        NONE = 0,
        PLAYER,
        ENEMY,
        BRIDGE,
        RIVER
    };

public:
    Grid() : _row(0), _col(0), _worldPos(0, 0), _walkable(true) {};
    Grid(int row, int col, const cocos2d::Vec2& worldPos)
        : _row(row), _col(col), _worldPos(worldPos), _regionType(RegionType::NONE), _walkable(true), _deployable(true)
    {
    };

    // 坐标
    int getRow() const { return _row; }
    int getCol() const { return _col; }
    cocos2d::Vec2 getWorldPos() const { return _worldPos; }

    // 区域语义
    void setRegionType(RegionType type) { _regionType = type; }
    RegionType getRegionType() const { return _regionType; }

    // 行为属性
    bool isWalkable() const { return _walkable; }
    bool isDeployable() const { return _deployable; }

    void setWalkable(bool v) { _walkable = v; }
    void setDeployable(bool v) { _deployable = v; }

private:
    int _row;
    int _col;
    cocos2d::Vec2 _worldPos;

    RegionType _regionType;

    bool _walkable;
    bool _deployable;
};

#endif // GRID_H
