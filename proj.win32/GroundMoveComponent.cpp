#include "GroundMoveComponent.h"
#include "TroopBase.h"
#include "BuildingBase.h"
#include "IAttackable.h"
#include "cocos2d.h"

USING_NS_CC;

GroundMoveComponent::GroundMoveComponent()
{
}

void GroundMoveComponent::onUpdateMove(TroopBase* owner, float dt)
{
    if (!owner || !_followTarget || owner->isDead())
    {
        this->stop();
        return;
    }

    // 检查目标是否死亡
    if (_followTarget->isDead())
    {
        this->stop();
        return;
    }

    // 获取当前位置和目标位置
    Vec2 ownerPos = owner->getPosition();
    Vec2 targetPos = _followTarget->getWorldPosition();

    // 计算距离
    Vec2 direction = targetPos - ownerPos;
    float centerDistance = direction.length();

    // 如果已经非常接近，停止移动
    if (centerDistance <= 1.0f)
    {
        this->stop();
        return;
    }

    // 获取双方碰撞半径
    float ownerRadius = owner->getBodyRadius();
    float targetRadius = _followTarget->getBodyRadius();

    // 计算实际表面距离（考虑碰撞体积）
    float surfaceDistance = centerDistance - ownerRadius;

    // 如果已经达到攻击距离，停止移动
    if (surfaceDistance <= _desiredDistance)
    {
        this->stop();
        return;
    }

    // 计算需要移动的距离
    float distanceToMove = surfaceDistance - _desiredDistance;

    // 获取移动速度
    float moveSpeed = owner->getMoveSpeed();
    if (moveSpeed <= 0.0f)
    {
        moveSpeed = 100.0f; // 默认速度
    }

    // 计算本帧最大移动距离
    float maxMoveThisFrame = moveSpeed * dt;

    // 实际移动距离
    float actualMove = std::min(maxMoveThisFrame, distanceToMove);

    // 安全检查
    if (actualMove <= 0.0f || centerDistance <= 0.0f)
    {
        this->stop();
        return;
    }

    // 计算移动方向并更新位置
    direction.normalize();
    Vec2 newPos = ownerPos + direction * actualMove;
    owner->setPosition(newPos);

    // 调试信息：显示目标类型
    static float debugTimer = 0;
    debugTimer += dt;
    if (debugTimer > 1.5f)
    {
        debugTimer = 0;

        // 识别目标类型
        TroopBase* troopTarget = dynamic_cast<TroopBase*>(_followTarget);
        BuildingBase* buildingTarget = dynamic_cast<BuildingBase*>(_followTarget);

        const char* targetType = troopTarget ? "Troop" : (buildingTarget ? "Building" : "Unknown");

        CCLOG("GroundMove: %s following %s, Distance: %.1f/%.1f",
            owner->getName().c_str(), targetType, surfaceDistance, _desiredDistance);
    }
}

float GroundMoveComponent::calculateMovement(TroopBase* owner, const cocos2d::Vec2& targetPos)
{
    // 计算当前位置到目标的向量
    Vec2 direction = targetPos - owner->getPosition();
    float distance = direction.length();

    // 考虑碰撞半径的调整
    float adjustedDistance = distance - owner->getBodyRadius();

    return adjustedDistance;
}