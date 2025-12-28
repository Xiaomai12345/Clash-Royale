#include "BuildingAttackComponent.h"
#include "BuildingBase.h"
#include "Projectile.h"

BuildingAttackComponent::BuildingAttackComponent(float range, float interval, int damage, float projectileSpeed)
    : AttackComponent(range, interval)
    , _damage(damage)
    , _projectileSpeed(projectileSpeed)
{
}

void BuildingAttackComponent::doAttack(IAttackable* owner)
{
    BuildingBase* building = dynamic_cast<BuildingBase*>(owner);
    if (building)
    {
        if (!_target)
            return;

        Node* world = owner->getParent();
        if (!world) return;

        Vec2 startPos = owner->getPosition();
        startPos.y += 40;

        int damage = _damage;
        auto target = _target; // C++11 capture safe
 
        auto projectile = Projectile::create(
            _target,
            damage,
            _projectileSpeed,
            [target, damage]() // 捕获 target 指针
            {
                if (target)
                {
                    target->takeDamage(damage);
                }
            });

        if (projectile)
        {
            projectile->setPosition(startPos);
            world->addChild(projectile, 1000); 
        }

        CCLOG("BuildingAttack: building %p fired at %p", owner, _target);
    }
}