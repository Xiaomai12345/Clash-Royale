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

void MoveComponent::followTarget(IAttackable* target, float desiredDistance)
{
    _followTarget = target;
    _desiredDistance = desiredDistance;
    _moving = (target != nullptr);
}

void MoveComponent::stop()
{
    _moving = false;
    _followTarget = nullptr;
}
void MoveComponent::update(TroopBase* owner, float dt)
{
    if (!owner || owner->isDead())
        return;

    if (!_moving || !_followTarget)
        return;

    // 检查目标是否有效
    if (_followTarget->isDead())
    {
        stop();
        return;
    }
    // 确保目标仍然在场景中
    // 由于IAttackable没有getParent方法，我们需要通过其他方式检查
    // 这里假设如果目标还活着，就继续移动

    // 调用子类实现的具体移动逻辑
    onUpdateMove(owner, dt);
}