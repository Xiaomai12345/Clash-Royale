#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include "cocos2d.h"
#include "TowerBase.h"
class TowerBase;

class Battlefield : public cocos2d::Node
{
public:
    CREATE_FUNC(Battlefield);

    virtual bool init() override;

    // 地图初始化
    void setupBattlefield(int level = 1);

    // 位置有效性检查
    bool isValidDeployPosition(const cocos2d::Vec2& worldPos, int playerId) const;

    // 获取地图尺寸
    cocos2d::Size getMapSize() const { return _mapSize; }
    cocos2d::Size getGridSize() const { return _gridSize; }

    // 调试绘制
    void enableDebugDraw(bool enable);
    bool isDebugDrawEnabled() const { return _debugEnabled; }

    // 获取塔
    cocos2d::Vector<TowerBase*> getAllTowers() const { return _allTowers; }

private:
    void createBackground();
    void createTowers();
    void createDebugLayer();

private:
    cocos2d::Size _mapSize;
    cocos2d::Size _gridSize;

    // 地图元素
    cocos2d::Sprite* _background;
    cocos2d::DrawNode* _debugDrawNode;

    // 塔
    cocos2d::Vector<TowerBase*> _allTowers;

    // 调试
    bool _debugEnabled;
};

#endif // BATTLEFIELD_H