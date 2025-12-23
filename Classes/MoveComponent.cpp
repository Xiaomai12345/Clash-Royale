#include "MoveComponent.h"
#include "TroopBase.h"
#include "IAttackable.h"
#include "cocos2d.h"

USING_NS_CC;

MoveComponent::MoveComponent()
    : _followTarget(nullptr)
    , _desiredDistance(0.0f)
    , _moving(false)
{
}

MoveComponent::~MoveComponent()
{
}

/*
 * 跟随一个目标单位（可以是士兵或建筑）
 * @param target          要跟随的目标（IAttackable接口）
 * @param desiredDistance 希望与目标保持的最小距离（攻击距离）
 */
void MoveComponent::followTarget(IAttackable* target, float desiredDistance)
{
    _followTarget = target;
    _desiredDistance = desiredDistance;
    _moving = (target != nullptr);
}

/*
 * 停止移动
 * 清除当前跟随目标
 */
void MoveComponent::stop()
{
    _moving = false;
    _followTarget = nullptr;

    CCLOG("MoveComponent: Stopped moving");
}

/*
 * 每帧更新（模板方法）
 */
void MoveComponent::update(TroopBase* owner, float dt)
{
    if (!owner || owner->isDead())
        return;

    if (!_moving || !_followTarget)
        return;

    // 检查目标是否有效
    if (_followTarget->isDead())
    {
        CCLOG("MoveComponent: Target is dead");
        stop();
        return;
    }

    // 确保目标仍然在场景中
    // 由于IAttackable没有getParent方法，我们需要通过其他方式检查
    // 这里假设如果目标还活着，就继续移动

    // 调用子类实现的具体移动逻辑
    onUpdateMove(owner, dt);
}