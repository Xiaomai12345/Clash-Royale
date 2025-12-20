#include "RangedAttackComponent.h"
#include "TroopBase.h"

RangedAttackComponent::RangedAttackComponent(float range, float interval, int damage, float projectileSpeed)
    : AttackComponent(range, interval), _projectileSpeed(projectileSpeed), _damage(damage)
{
}

void RangedAttackComponent::doAttack(IAttackable* owner)
{
    // 如果 owner 是 TroopBase，则调用你的 TroopBase 版本
    TroopBase* troop = dynamic_cast<TroopBase*>(owner);
    if (troop)
    {
        doAttack(troop);
    }
    // 否则可以加日志或其他处理
}

void RangedAttackComponent::doAttack(TroopBase* owner)
{
    if (!_target || _target->isDead())
        return;

    // 计算发射子弹，远程攻击的具体实现可以加上子弹的飞行逻辑
    CCLOG("RangedAttack: %p attacks %p with %d damage, projectile speed %f", owner, _target, _damage, _projectileSpeed);
    // 扣除目标血量
    _target->takeDamage(_damage);

    // 这里可以拓展远程攻击的子弹飞行逻辑等，例如生成一个子弹对象并发射
}
