#include "RangedAttackComponent.h"
#include "TroopBase.h"
#include "Projectile.h"

RangedAttackComponent::RangedAttackComponent(float range, float interval, int damage, float projectileSpeed)
    : AttackComponent(range, interval), _projectileSpeed(projectileSpeed), _damage(damage)
{
}

void RangedAttackComponent::doAttack(IAttackable* owner)
{
    // 将 owner 转为 TroopBase
    TroopBase* troop = dynamic_cast<TroopBase*>(owner);
    if (troop)
    {
        doAttack(troop);
    }
}

void RangedAttackComponent::doAttack(TroopBase* owner)
{
    if (!_target || _target->isDead())
        return;

    // 1. 获取发射位置（世界坐标或父节点坐标）
    // 假设 owner 和 Projectile 都将添加到同一个父节点 (Battlefield)
    Node* world = owner->getParent();
    if (!world) return;

    Vec2 startPos = owner->getPosition();
    startPos.y += 20; // 简单的发射点高度偏移

    // 2. 创建弹道
    // 注意：Lambda 必须按值捕获 damage，不能捕获 this，防止发射者死亡后组件被销毁导致野指针
    int damage = _damage;

    auto projectile = Projectile::create(_target, damage, _projectileSpeed, [target = _target, damage]() {
        // 命中回调
        if (target && !target->isDead())
        {
            target->takeDamage(damage);
        }
    });

    if (projectile)
    {
        projectile->setPosition(startPos);
        // 添加到场景中，ZOrder 设高一点确保在单位上方
        world->addChild(projectile, 1000); 
    }

    CCLOG("RangedAttack: %p fired projectile at %p", owner, _target);
}
