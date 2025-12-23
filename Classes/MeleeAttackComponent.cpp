#include "MeleeAttackComponent.h"
#include "TroopBase.h"

MeleeAttackComponent::MeleeAttackComponent(float range, float interval, int damage)
    : AttackComponent(range, interval)
    , _damage(damage)
{
}

void MeleeAttackComponent::doAttack(IAttackable* owner)
{
	TroopBase* troop = dynamic_cast<TroopBase*>(owner);// 确保 owner 是 TroopBase 类型
    if (troop)
    {
        if (!_target || _target->isDead())
            return;

        // 近战：直接扣血
        _target->takeDamage(_damage);

        CCLOG("MeleeAttack: %p hit %p for %d damage",
            owner, _target, _damage);
    }
}
