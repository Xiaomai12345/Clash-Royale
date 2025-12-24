#include "AirMoveComponent.h"
#include "TroopBase.h"
#include "BuildingBase.h"
#include "IAttackable.h"
#include "cocos2d.h"

USING_NS_CC;

AirMoveComponent::AirMoveComponent()
{
}

void AirMoveComponent::onUpdateMove(TroopBase* owner, float dt)
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

    // 获取自身半径
    float ownerRadius = owner->getBodyRadius();

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

    Node* worldNode = owner->getParent();//世界中的节点

    for (Node* node : worldNode->getChildren())//遍历来防止模型碰撞
    {
        // 确保每个节点是可攻击的目标
        auto target = dynamic_cast<IAttackable*>(node);
        if (!target)
            continue; // 如果不是有效的攻击目标，跳过

        // 忽略自己，或者是已经死亡的目标
        if (target == owner || target->isDead())
            continue;

        if (owner->getMoveType() != target->getMoveType())//移动类型不同，不需要判断碰撞。
            continue;

        // 计算目标与当前单位的距离
        float dist = ownerPos.distance(target->getPosition());
        float hitRange = ownerRadius + target->getBodyRadius();  // 碰撞检测范围：当前单位半径 + 目标单位半径

        // 如果在碰撞范围内，进行碰撞判断
        if (dist <= hitRange)
        {
            Vec2 pushDirection = ownerPos - target->getPosition();  // 从目标指向当前单位
            pushDirection.normalize();  // 归一化，使得方向为单位向量

            // 计算推动的力度（可以根据需要调整）
            float pushStrength = 2.0f;  // 推动力度，可以根据实际情况调节
            Vec2 ownerNewPos = ownerPos + pushDirection * pushStrength;
            Vec2 targetNewPos = target->getPosition() - pushDirection * pushStrength;

            // 更新单位的位置，确保它们不再相撞
            owner->setPosition(ownerNewPos);
            target->setPosition(targetNewPos);

            CCLOG("Collision resolved: %s and %s pushed apart", owner->getName().c_str(), target->getName().c_str());
        }
    }

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

        CCLOG("AirMoveComponent: %s following %s, Distance: %.1f/%.1f",
            owner->getName().c_str(), targetType, surfaceDistance, _desiredDistance);
    }
}

