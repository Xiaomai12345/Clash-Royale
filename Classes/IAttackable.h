// IAttackable.h
#ifndef __I_ATTACKABLE_H__
#define __I_ATTACKABLE_H__

#include "cocos2d.h"
USING_NS_CC;

enum class ECamp//定义阵营
{
    LEFT,//0
    RIGHT//1
};

enum class State {//定义状态类型
    IDLE,       // 闲置
    ALERT,      // 警戒（发现目标）
    FOLLOWING,  // 跟随目标
    ATTACKING   // 攻击
};

enum class AttackType//定义攻击类型 
{
Both,
Building,
};

class IAttackable:public Node
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
    //减速BUFF
    virtual void applySlow(float ratio, float duration) = 0;




    //获取阵营,获取位置
    virtual ECamp getCamp() const = 0;
    virtual AttackType getAttackType() const = 0;
}; 

#endif
