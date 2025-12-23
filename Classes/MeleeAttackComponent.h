#ifndef __MELEE_ATTACK_COMPONENT_H__
#define __MELEE_ATTACK_COMPONENT_H__

#include "AttackComponent.h"
class TroopBase;
// =========================
// MeleeAttackComponent
// =========================
//
// 近战攻击组件
//
// 特点：
// - 攻击范围小
// - 直接对目标造成伤害
//
class MeleeAttackComponent : public AttackComponent
{
public:
    // range: 攻击范围
    // interval: 攻击间隔
    // damage: 每次伤害
    MeleeAttackComponent(float range, float interval, int damage);

protected:
    // 真正执行一次攻击
    virtual void doAttack(IAttackable* owner) override;

private:
    int _damage = 0;
};

#endif // __MELEE_ATTACK_COMPONENT_H__
