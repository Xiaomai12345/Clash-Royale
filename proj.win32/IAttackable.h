// IAttackable.h
#ifndef __I_ATTACKABLE_H__
#define __I_ATTACKABLE_H__

#include "cocos2d.h"
USING_NS_CC;

enum class ECamp//定义阵营
{
    LEFT,
    RIGHT
};
class IAttackable
{
public:
    virtual ~IAttackable() = default;

    // 是否死亡
    virtual bool isDead() const = 0;

    // 世界坐标
    virtual Vec2 getWorldPosition() const = 0;

    // 碰撞 / 判定半径
    virtual float getBodyRadius() const = 0;

    // 受伤
    virtual void takeDamage(int damage) = 0;
};

#endif
