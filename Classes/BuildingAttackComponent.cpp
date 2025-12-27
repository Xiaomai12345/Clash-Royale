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
        
        // 使用 WeakPtr 或者简单的安全检查逻辑
        // 在这里，我们将 target 捕获进 lambda，而不是依赖 this->_target
        // 这样即使组件本身被销毁，或者 this->_target 变了，lambda 依然操作的是当初发射时的那个目标
        // 但是，如果 target 对象本身被 delete 了，target 指针依然会悬空
        // 为了安全，最根本的解决办法是在 target 被 delete 前停止所有 projectile，
        // 或者在 Projectile 类中处理 target 的有效性（例如 Projectile::update 中检查）
        
        auto projectile = Projectile::create(
            _target,
            damage,
            _projectileSpeed,
            [target, damage]() // 捕获 target 指针
            {
                // 由于 Projectile 内部 retain 了 target，
                // 所以只要 Projectile 还在，target 指针指向的内存就是有效的。
                // 我们只需要检查 target 的逻辑状态（是否死亡，是否在场景中）
                if (target && !target->isDead() && target->getParent())
                {
                    target->takeDamage(damage);
                }
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