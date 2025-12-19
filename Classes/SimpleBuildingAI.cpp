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
        CCLOG("SimpleBuildingAI: No attack component found");
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
            // 目标在检测范围内但不在攻击范围内
            CCLOG("SimpleBuildingAI: Target out of range");
            clearTarget();
            attackComponent->clearTarget();
        }
    }
    else
    {
        // 目标无效，清除
        clearTarget();
        attackComponent->clearTarget();
    }

    // 需要寻找新目标
    if (_searchCooldown <= 0.0f)
    {
        IAttackable* newTarget = findEnemyInRange(owner);
        if (newTarget)
        {
            _target = newTarget;
            attackComponent->setTarget(newTarget);
            CCLOG("SimpleBuildingAI: New target found at distance %.1f",
                calculateDistance(owner, newTarget));
        }
        else
        {
            CCLOG("SimpleBuildingAI: No enemy found in range");
        }

        _searchCooldown = SEARCH_INTERVAL;
    }
}

IAttackable* SimpleBuildingAI::findEnemyInRange(BuildingBase* owner)
{
    if (!owner->getParent())
        return nullptr;

    Vec2 towerPos = owner->getPosition();
    IAttackable* closestEnemy = nullptr;
    float closestDistance = FLT_MAX;

    // 遍历所有子节点
    auto children = owner->getParent()->getChildren();
    for (auto child : children)
    {
        // 跳过自己
        if (child == owner)
            continue;

        // 只查找TroopBase（士兵）
        TroopBase* troop = dynamic_cast<TroopBase*>(child);
        if (!troop || troop->isDead())
            continue;

        // 检查是否是敌人（不同阵营）
        if (troop->getCamp() != owner->getCamp())
        {
            float distance = towerPos.distance(troop->getPosition());

            if (distance <= _detectionRange && distance < closestDistance)
            {
                closestDistance = distance;
                closestEnemy = troop;
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
        return distance <= effectiveRange;
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