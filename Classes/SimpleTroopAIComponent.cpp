#include "SimpleTroopAIComponent.h"
#include "TroopBase.h"
#include "MoveComponent.h"
#include "AttackComponent.h"

SimpleTroopAIComponent::SimpleTroopAIComponent()
{
}

void SimpleTroopAIComponent::update(TroopBase* owner, float dt)
{
    auto move = owner->getMoveComponent();
    auto attack = owner->getAttackComponent();
    if (!_target || _target->isDead())
    {
        attack->setTarget(NULL);
        return;
    }

    if (!move || !attack)
    {
        CCLOG("move and attack");
        return;
    }

    // 1️⃣ 把目标同步给攻击组件
    attack->setTarget(_target);

    // 2️⃣ 判断是否在攻击范围
    if (attack->isTargetInRange(owner))
    {
        // 在范围内 → 停止移动
        move->stop();
    }
    else
    {
        // 不在范围 → 追击
        move->followTarget(_target, attack->getAttackRange());
    }
}
