#ifndef __BUILDING_ATTACK_COMPONENT_H__  
#define __BUILDING_ATTACK_COMPONENT_H__

#include "AttackComponent.h"
class BuildingBase;  

//
// 建筑攻击组件
// 攻击范围中等
// 远程攻击
// 固定位置攻击
//
class BuildingAttackComponent : public AttackComponent 
{
public:

    BuildingAttackComponent(float range, float interval, int damage, float projectileSpeed);

protected:
    // 真正执行一次攻击
    virtual void doAttack(IAttackable* owner) override;

private:
    int _damage = 0;
    float _projectileSpeed = 0.0f;
};

#endif // __BUILDING_ATTACK_COMPONENT_H__  