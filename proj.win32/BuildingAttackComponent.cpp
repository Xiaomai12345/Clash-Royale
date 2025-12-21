#include "BuildingAttackComponent.h"  // ✅ 修正：头文件名
#include "BuildingBase.h"  // ✅ 修正：头文件名

BuildingAttackComponent::BuildingAttackComponent(float range, float interval, int damage)  // ✅ 修正：构造函数名
    : AttackComponent(range, interval)
    , _damage(damage)
{
}

void BuildingAttackComponent::doAttack(IAttackable* owner)
{
    BuildingBase* building = dynamic_cast<BuildingBase*>(owner);  // ✅ 修正：变量名和类型
    if (building)
    {
        if (!_target || _target->isDead())
            return;

        _target->takeDamage(_damage);  // 建筑直接扣血

        CCLOG("BuildingAttack: building %p hit %p for %d damage",
            owner, _target, _damage);
    }
}