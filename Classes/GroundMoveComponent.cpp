#include "GroundMoveComponent.h"
#include "TroopBase.h"
#include "BuildingBase.h"
#include "IAttackable.h"

USING_NS_CC;

GroundMoveComponent::GroundMoveComponent()
{
}

void GroundMoveComponent::onUpdateMove(TroopBase* owner, float dt)
{
    if (!owner || !_followTarget || owner->isDead())
    {
        stop();
        return;
    }

    if (_followTarget->isDead())
    {
        stop();
        return;
    }

    Vec2 ownerPos = owner->getPosition();
    Vec2 targetPos = _followTarget->getWorldPosition();

    Vec2 toTarget = targetPos - ownerPos;
    float dist = toTarget.length();
    if (dist < 0.001f)
        return;

    float radius = owner->getBodyRadius();
    float surfaceDist = dist - radius;

    // 已进入攻击距离
    if (surfaceDist <= _desiredDistance)
    {
        stop();
        return;
    }

    float speed = owner->getMoveSpeed();
    Vec2 desiredDir = toTarget.getNormalized();
    Vec2 desiredMove = desiredDir * speed * dt;

    Vec2 finalMove = desiredMove;

    Node* world = owner->getParent();
    if (!world)
        return;

    bool hasCollision = false;
    Vec2 collisionNormal;

    // =====================
    // 碰撞检测（只检测障碍，不含目标）
    // =====================
    for (Node* node : world->getChildren())
    {
        auto other = dynamic_cast<IAttackable*>(node);
        if (!other || other == owner || other == _followTarget || other->isDead())
            continue;

        if (owner->getMoveType() != other->getMoveType())
            continue;

        Vec2 diff = (ownerPos + finalMove) - node->getPosition();
        float minDist = radius + other->getBodyRadius();
        float d = diff.length();

        if (d < minDist && d > 0.001f)
        {
            hasCollision = true;
            collisionNormal = diff / d; // 法线
            break;
        }
    }

    // =====================
    // 状态切换
    // =====================
    if (hasCollision && !_isOrbiting)
    {
        // 第一次进入绕行：计算一次切线方向并锁死
        Vec2 tangent(-collisionNormal.y, collisionNormal.x);

        // 决定左绕还是右绕
        float side = desiredMove.dot(tangent);
        if (side < 0.f)
            tangent = -tangent;

        _orbitDir = tangent.getNormalized();
        _isOrbiting = true;
        _orbitTimer = _minOrbitTime;
    }

    // =====================
    // 绕行逻辑
    // =====================
    if (_isOrbiting)
    {
        _orbitTimer -= dt;

        // 🔒 完全忽略目标方向
        finalMove = _orbitDir * speed * dt;

        // 条件满足才退出绕行
        if (!hasCollision && _orbitTimer <= 0.f)
        {
            _isOrbiting = false;
        }
    }

    owner->setPosition(ownerPos + finalMove);
}
