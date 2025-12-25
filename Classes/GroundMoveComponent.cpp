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
        // 关键修复：检查是否真的需要过河
        // 只有当“我”和“目标”之间隔着河时，才进入 ToBridge 状态
        // 否则直接走直线（例如已经投放在敌方区域）
        if (world->hasRiverBetween(ownerPos, targetPos))
        {
            _bridgeState = BridgeState::ToBridge;
            _cachedBridgePos = Vec2(
                world->getNearestBridgeX(ownerPos),
                world->getNearestBridgeY(ownerPos)
            );
        }
        else
        {
            // 在同侧，不需要过桥逻辑
            // 保持 None 状态，后续会直接朝 targetPos 移动
        }
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

    float ySign = (moveDir.y >= 0.f) ? 1.f : -1.f;
    Vec2 bodyOffset = Vec2(0, ySign) * ownerRadius;

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



	Vec2 moveDirection = finalPos - ownerPos;
    owner->setPosition(finalPos);
	moveDirection.normalize();




    // =========================
    // 碰撞推挤（单位之间）
    // =========================



    Node* parent = owner->getParent();
    if (!parent)
        return;

    Vec2 selfPos = owner->getPosition();
    Vec2 desiredDir = moveDirection;


    for (Node* node : parent->getChildren())
    {
        auto target = dynamic_cast<IAttackable*>(node);
        if (!target || target == owner || target->isDead())
            continue;

        if (owner->getMoveType() != target->getMoveType())
            continue;

        Vec2 targetPos = target->getPosition();
        float targetRadius = target->getBodyRadius();

        float dist = selfPos.distance(targetPos);
        float hitRange = ownerRadius + targetRadius;

        if (dist <= hitRange && dist > 0.001f)
        {
            // =========================
            // 1️⃣ 法线方向（分离）
            // =========================
            Vec2 normal = (selfPos - targetPos).getNormalized();

            float overlap = hitRange - dist;
            float separateStrength = std::max(1.0f, overlap * 0.6f);
            Vec2 separateOffset = normal * separateStrength;

            // =========================
            // 2️⃣ 切线方向（滑动）
            // =========================
            Vec2 tangent(-normal.y, normal.x);

            // 根据当前移动方向决定切线正负
            if (tangent.dot(desiredDir) < 0)
                tangent = -tangent;

            float slideStrength = separateStrength * 0.8f;
            Vec2 slideOffset = tangent * slideStrength;

            // =========================
            // 3️⃣ 合成位移
            // =========================
            Vec2 candidatePos = selfPos + separateOffset + slideOffset;

            // =========================
            // 4️⃣ 安全校验
            // =========================
            Vec2 safePos = world->constrainPosition(candidatePos, selfPos);

            if (world->canWalk(safePos))
            {
                owner->setPosition(safePos);
                selfPos = safePos;
            }
        }
    }

}
