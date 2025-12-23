#pragma once
#include "cocos2d.h"

class IWalkableWorld
{
public:
    virtual ~IWalkableWorld() = default;

    virtual bool canWalk(const cocos2d::Vec2& worldPos) const = 0;
    virtual bool isBridge(const cocos2d::Vec2& worldPos) const = 0;
    virtual cocos2d::Vec2 constrainPosition(const cocos2d::Vec2& desired, const cocos2d::Vec2& current) const = 0;
    virtual float getNearestBridgeX(const cocos2d::Vec2& currentPos) const = 0;
    virtual float getNearestBridgeY(const cocos2d::Vec2& currentPos) const = 0;

};
