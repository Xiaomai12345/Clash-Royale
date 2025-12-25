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
        if (!_target || _target->isDead())
            return;

        // 1. 获取发射位置（世界坐标或父节点坐标）
        Node* world = owner->getParent();
        if (!world) return;

        Vec2 startPos = owner->getPosition();
        startPos.y += 40; // 塔比较高，发射点稍微高一点

        // 2. 创建弹道
        int damage = _damage;
        auto target = _target; // C++11 capture safe
        
        auto projectile = Projectile::create(
            _target,
            damage,
            _projectileSpeed,
            [this, damage]()
            {
                if (!_target)
                    return;

                if (_target->isDead())
                {
                    _target = nullptr;
                    return;
                }

                _target->takeDamage(damage);
            });

        if (projectile)
        {
            projectile->setPosition(startPos);
            // 添加到场景中，ZOrder 设高一点
            world->addChild(projectile, 1000); 
        }

        CCLOG("BuildingAttack: building %p fired at %p", owner, _target);
    }
}