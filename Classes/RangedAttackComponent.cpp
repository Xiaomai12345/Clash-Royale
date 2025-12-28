#include "RangedAttackComponent.h"
#include "TroopBase.h"
#include "Projectile.h"

RangedAttackComponent::RangedAttackComponent(float range, float interval, int damage, float projectileSpeed)
    : AttackComponent(range, interval), _projectileSpeed(projectileSpeed), _damage(damage)
{
}

void RangedAttackComponent::doAttack(IAttackable* owner)
{
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


    Node* world = owner->getParent();
    if (!world) return;

    Vec2 startPos = owner->getPosition();
    startPos.y += 20; 

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
        world->addChild(projectile, 1000); 
    }

    CCLOG("RangedAttack: %p fired projectile at %p", owner, _target);
}
