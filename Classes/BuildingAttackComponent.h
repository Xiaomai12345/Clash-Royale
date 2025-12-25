#ifndef __BUILDING_ATTACK_COMPONENT_H__  // ✅ 修正：宏名改为BUILDING_ATTACK
#define __BUILDING_ATTACK_COMPONENT_H__

#include "AttackComponent.h"
class BuildingBase;  // ✅ 修正：类名拼写

//
// 建筑攻击组件
// 特点：
// - 攻击范围中等
// - 远程攻击
// - 固定位置攻击
//
class BuildingAttackComponent : public AttackComponent  // ✅ 修正：类名拼写
{
public:
    // range: 攻击范围
    // interval: 攻击间隔
    // damage: 每次伤害
    // projectileSpeed: 弹道速度
    BuildingAttackComponent(float range, float interval, int damage, float projectileSpeed);

protected:
    // 真正执行一次攻击
    virtual void doAttack(IAttackable* owner) override;

private:
    int _damage = 0;
    float _projectileSpeed = 0.0f;
};

#endif // __BUILDING_ATTACK_COMPONENT_H__  // ✅ 修正：宏名