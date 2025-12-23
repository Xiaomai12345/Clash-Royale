#include "GroundMoveComponent.h"
#include "TroopBase.h"
#include "IAttackable.h"
#include "IWalkableWorld.h"
#include "PrincessTower.h"
#include "KingdomTower.h"

USING_NS_CC;

GroundMoveComponent::GroundMoveComponent()
    : _bridgeState(BridgeState::None)
    , _riverDir(Vec2::ZERO)
    , _cachedBridgePos(Vec2::ZERO)
{
}

void GroundMoveComponent::onUpdateMove(TroopBase* owner, float dt)
{
    if (!owner || !_followTarget || owner->isDead())
        return;

    IWalkableWorld* world = owner->getWorld();
    if (!world)
        return;

    Vec2 ownerPos = owner->getPosition();
    Vec2 targetPos = _followTarget->getWorldPosition();

    Vec2 toTarget = targetPos - ownerPos;
    float centerDistance = toTarget.length();
    if (centerDistance <= 1.0f)
        return;

    float ownerRadius = owner->getBodyRadius();
    float targetRadius = _followTarget->getBodyRadius();

    // 如果已经达到攻击距离，停止移动 
    float surfaceDistance = centerDistance - targetRadius;
    if (surfaceDistance <= _desiredDistance)
    {
        this->stop();
        return;
    }

    // =========================
    // 桥目标判断
    // =========================
    bool bridgeTarget =
        dynamic_cast<PrincessTower*>(_followTarget) ||
        dynamic_cast<KingdomTower*>(_followTarget);

    if (bridgeTarget && _bridgeState == BridgeState::None)
    {
        _bridgeState = BridgeState::ToBridge;
        _cachedBridgePos = Vec2(
            world->getNearestBridgeX(ownerPos),
            world->getNearestBridgeY(ownerPos)
        );
    }


    // =========================
    // 目标检查
    // =========================
    if (_followTarget->isDead())
    {
        this->stop();
        return;
    }

    // =========================
    // 距离 & 移动量计算
    // =========================
    float moveSpeed = owner->getMoveSpeed();
    float maxMove = moveSpeed * dt;

    float moveDist = std::min(maxMove, surfaceDistance - _desiredDistance);
    if (moveDist <= 0.f)
        return;

    Vec2 moveDir = toTarget;
    moveDir.normalize();

    // =========================
    // ToBridge 状态
    // =========================
    if (_bridgeState == BridgeState::ToBridge)
    {
        Vec2 toBridge = _cachedBridgePos - ownerPos;
        float distToBridge = std::max(0.f, toBridge.length() - ownerRadius);

        if (distToBridge <= 0.1f)
        {
            _bridgeState = BridgeState::OnBridge;
        }
        else
        {
            moveDir = toBridge.getNormalized();
            moveDist = std::min(maxMove, distToBridge);
        }
    }

    Vec2 newPos = ownerPos + moveDir * moveDist;
    Vec2 bodyOffset = moveDir * ownerRadius;

    Vec2 finalPos = world->constrainPosition(newPos, ownerPos);

    // =========================
    // 河道 / 桥口滑行
    // =========================
    if (!world->canWalk(newPos + bodyOffset) &&
        _bridgeState != BridgeState::OnBridge)
    {
        float bridgeX = world->getNearestBridgeX(ownerPos);
        float bridgeWidth = 40.5f;

        float bridgeLeft = bridgeX - bridgeWidth * 0.5f;
        float bridgeRight = bridgeX + bridgeWidth * 0.5f;

        float bodyLeft = ownerPos.x - ownerRadius;
        float bodyRight = ownerPos.x + ownerRadius;

        float diffX = 0.f;
        if (bodyLeft < bridgeLeft)
            diffX = bridgeLeft - bodyLeft;
        else if (bodyRight > bridgeRight)
            diffX = bridgeRight - bodyRight;

        if (std::abs(diffX) > 0.1f)
        {
            float dirX = diffX > 0 ? 1.f : -1.f;
            _riverDir = Vec2(dirX, 0.f);

            Vec2 slidePos = ownerPos + _riverDir * maxMove;
            if (world->canWalk(slidePos))
                finalPos = slidePos;
            else
                finalPos = ownerPos;
        }
        else
        {
            finalPos = ownerPos;
        }
    }

    // =========================
    // 桥上约束
    // =========================
    if (world->isBridge(ownerPos) || _bridgeState == BridgeState::OnBridge)
    {
        Vec2 constrained = world->constrainPosition(newPos, ownerPos);

        if (constrained == ownerPos && !world->canWalk(newPos))
        {
            Vec2 bridgeDir = (targetPos.y > ownerPos.y)
                ? Vec2(0, 1)
                : Vec2(0, -1);

            Vec2 safeMove = ownerPos + bridgeDir * maxMove;
            finalPos = world->constrainPosition(safeMove, ownerPos);
        }
        else
        {
            finalPos = constrained;
        }

        if (!world->isBridge(finalPos))
        {
            _bridgeState = BridgeState::Passed;
        }
    }
    else if (world->isBridge(finalPos))
    {
        finalPos = world->constrainPosition(finalPos, ownerPos);
        _bridgeState = BridgeState::OnBridge;
    }

    // =========================
    // 微调防卡死
    // =========================
    if (finalPos.distance(ownerPos) < 0.01f)
    {
        Vec2 micro = ownerPos + moveDir * 0.1f;
        if (world->canWalk(micro))
            finalPos = micro;
    }

    owner->setPosition(finalPos);

    // =========================
    // 碰撞推挤（单位之间）
    // =========================
    Node* parent = owner->getParent();
    if (parent)
    {
        Vec2 selfPos = owner->getPosition();
        for (Node* node : parent->getChildren())
        {
            auto target = dynamic_cast<IAttackable*>(node);
            if (!target || target == owner || target->isDead())
                continue;

            // 不同移动类型不碰撞（如空军与地面）
            if (owner->getMoveType() != target->getMoveType())
                continue;

            float dist = selfPos.distance(target->getPosition());
            float hitRange = ownerRadius + target->getBodyRadius();

            if (dist <= hitRange && dist > 0.001f)
            {
                Vec2 pushDir = (selfPos - target->getPosition()).getNormalized();

                // 计算重叠量，给予一个推力
                float overlap = hitRange - dist;
                float pushStrength = std::max(1.0f, overlap * 0.5f);

                // 计算推挤后的新位置
                Vec2 newSelfPos = selfPos + pushDir * pushStrength;

                // 确保推挤后的位置合法（不会被推到河里）
                Vec2 safeSelfPos = world->constrainPosition(newSelfPos, selfPos);

                if (world->canWalk(safeSelfPos))
                {
                    owner->setPosition(safeSelfPos);
                    // 更新 selfPos 以便后续循环使用最新位置
                    selfPos = safeSelfPos;
                }
            }
        }
    }
}
