#include "SimpleTroopAIComponent.h"
#include "TroopBase.h"
#include "MoveComponent.h"
#include "AttackComponent.h"
#include"PrincessTower.h"
#include"KingdomTower.h"
SimpleTroopAIComponent::SimpleTroopAIComponent()
{
}
void SimpleTroopAIComponent::update(TroopBase* owner, float dt)
{
    auto move = owner->getMoveComponent();
    auto attack = owner->getAttackComponent();

    if (!_target) {
		owner->setState(State::IDLE);//设置状态为闲置
    }

    _target = checkAlertRange(owner);

    if (!_target) {
        return;
    }

    if (!move || !attack)
    {
        CCLOG("move and attack components are not valid");
        return;
    }


    attack->setTarget(_target);

    // 2️⃣ 判断是否在攻击范围
    if (attack->isTargetInRange(owner))
    {
		owner->setState(State::ATTACKING);//设置状态为攻击
        // 在范围内 → 停止移动
        move->stop();
    }
    else
    {
        // 不在范围 → 追击
        move->followTarget(_target, attack->getAttackRange());
    }
}

IAttackable* SimpleTroopAIComponent::checkAlertRange(TroopBase* owner)
{
    Node* parent = owner->getParent();
    if (!parent)
        return nullptr;

    Vec2 ownerPos = owner->getPosition();
    IAttackable* BaseTarget = nullptr;
    IAttackable* InRangeTagrget = nullptr;
    float distancetoBaseTarget = 999999.f; // 极大初始值
    float InRangeDistance = 999999.f;
    float _alertRange = owner->getAlertRange();

    for (Node* node : parent->getChildren())
    {
        auto target = dynamic_cast<IAttackable*>(node);
        if (!target || target->getCamp() == owner->getCamp())
            continue;

		if (owner->getState()== State::ATTACKING && target != _target)//如果当前处于攻击状态，并且目标不是当前目标，则跳过
            continue;

        // 判断是否是公主塔
        if (target == dynamic_cast<PrincessTower*>(node)|| target == dynamic_cast<KingdomTower*>(node))
        {
            float temp = ownerPos.distance(target->getPosition());
            if (temp < distancetoBaseTarget)
            {
                distancetoBaseTarget = temp;
                BaseTarget = target;
            }
        }

        // 检查目标是否在警戒范围内
        float dist = ownerPos.distance(target->getPosition());
        if (dist - _alertRange <= 0.1f)
        {
            if (target == dynamic_cast<TroopBase*>(node) && owner->getAttackType() == AttackType::Building)
                continue;
            
            if (target->getMoveAttacked() != MoveAttack::Both && owner->getMoveAttack() != MoveAttack::Both && target->getMoveAttacked() != owner->getMoveAttack())//表明攻击类型与被攻击类型不匹配
                continue;

            if (dist < InRangeDistance)
            {
                InRangeDistance = dist;
                InRangeTagrget = target;
            }
        }
    }

    if (InRangeTagrget != nullptr)
        return InRangeTagrget;

    return BaseTarget;
}
