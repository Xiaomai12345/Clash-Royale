#include "SimpleBuildingAI.h"
#include "BuildingBase.h"
#include "TroopBase.h"
#include "AttackComponent.h"
#include "cocos2d.h"

USING_NS_CC;

// 搜索间隔：每0.5秒搜索一次敌人
const float SimpleBuildingAI::SEARCH_INTERVAL = 0.5f;

SimpleBuildingAI::SimpleBuildingAI(float detectionRange)
    : _detectionRange(detectionRange)
    , _searchCooldown(0.0f)
{
}

void SimpleBuildingAI::update(BuildingBase* owner, float dt)
{
    if (!owner || owner->isDead())
        return;

    // 获取攻击组件
    AttackComponent* attackComponent = owner->getAttackComponent();
    if (!attackComponent)
    {
        return;
    }

    // 更新搜索冷却
    _searchCooldown -= dt;

    // 检查当前目标是否有效
    if (_target && !_target->isDead())
    {
        // 检查目标是否在攻击范围内
        if (isValidTarget(owner, _target))
        {
            // 目标有效且可攻击，无需重新搜索
            attackComponent->setTarget(_target);
            return;
        }
        else
        {
            clearTarget();
            attackComponent->clearTarget();
            owner->setState(State::IDLE); // 立即停止攻击状态
        }
    }
    else
    {
        // 目标无效，清除
        clearTarget();
        attackComponent->clearTarget();
        // 如果之前有目标但现在没了（例如目标刚死），也应立即切回 IDLE
        // 但为了避免在搜索间隔期间反复切换，这里可以暂时不强制设为 IDLE，
        // 而是让下面的搜索逻辑决定。不过为了保险起见，没目标就该 IDLE。
        if (owner->getState() == State::ATTACKING)
        {
            owner->setState(State::IDLE);
        }
    }

    // 需要寻找新目标
    if (_searchCooldown <= 0.0f)
    {
        IAttackable* newTarget = findEnemyInRange(owner);
        if (newTarget)
        {
            _target = newTarget;
            attackComponent->setTarget(newTarget);
            owner->setState(State::ATTACKING);
        }
        else
        {
            owner->setState(State::IDLE);
        }

        _searchCooldown = SEARCH_INTERVAL;
    }
}

IAttackable* SimpleBuildingAI::findEnemyInRange(IAttackable* owner)//寻找范围内的敌人
{
    if (!owner->getParent())
        return nullptr;

    Vec2 towerPos = owner->getPosition();
    IAttackable* closestEnemy = nullptr;
    float closestDistance = FLT_MAX;

    // 遍历所有子节点
    Node* parent = owner->getParent();
    if (!parent)
        return nullptr;
    for (Node* node : parent->getChildren())
    {
        // 跳过自己
        if (node == owner)
            continue;

        auto* target = dynamic_cast<IAttackable*>(node);
        if (!target || target->isDead())
            continue;

        if (owner->getState() == State::ATTACKING && target != _target)//如果当前处于攻击状态，并且目标不是当前目标，则跳过
            continue;

        // 检查是否是敌人（不同阵营）
        if (target->getCamp() != owner->getCamp())
        {
            if (target->getMoveAttacked() != MoveAttack::Both && owner->getMoveAttack() != MoveAttack::Both && target->getMoveAttacked() != owner->getMoveAttack())//表明攻击类型与被攻击类型不匹配
                continue;
            float distance = towerPos.distance(target->getPosition());

            if (distance <= _detectionRange && distance < closestDistance)
            {
                closestDistance = distance;
                closestEnemy = target;
            }
        }
    }

    return closestEnemy;
}

bool SimpleBuildingAI::isValidTarget(BuildingBase* owner, IAttackable* target) const
{
    if (!owner || !target || target->isDead())
        return false;

    // 检查距离
    float distance = calculateDistance(owner, target);
    auto attackComponent = owner->getAttackComponent();

    if (attackComponent)
    {
        // 使用攻击组件的攻击范围
        float attackRange = attackComponent->getAttackRange();
        float targetRadius = target->getBodyRadius();

        // 考虑目标半径的有效距离
        float effectiveRange = attackRange + targetRadius;
		return distance - effectiveRange <= 0.1f;//允许一点误差
    }

    return false;
}

float SimpleBuildingAI::calculateDistance(BuildingBase* owner, IAttackable* target) const
{
    if (!owner || !target)
        return FLT_MAX;

    Vec2 ownerPos = owner->getPosition();
    Vec2 targetPos = target->getWorldPosition();

    return ownerPos.distance(targetPos);
}