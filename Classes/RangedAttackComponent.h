#ifndef __RANGED_ATTACK_COMPONENT_H__
#define __RANGED_ATTACK_COMPONENT_H__

#include "AttackComponent.h"

class TroopBase; // 前向声明，修复C2061错误

// 远程攻击组件
// 负责远程攻击逻辑，判断目标是否在攻击范围内，并执行攻击
class RangedAttackComponent : public AttackComponent
{
public:
    RangedAttackComponent(float range, float interval, int damage, float projectileSpeed);
    virtual ~RangedAttackComponent() {}

    // 覆盖基类的纯虚函数
    virtual void doAttack(IAttackable* owner) override;

    // 可选：你自己的 TroopBase 版本
    void doAttack(TroopBase* owner);

protected:
    float _projectileSpeed; // 子弹速度（远程攻击的特性）
    int _damage = 0;
};

#endif // __RANGED_ATTACK_COMPONENT_H__
