#include "GroundMoveComponent.h"
#include "TroopBase.h"
#include "IAttackable.h"
#include "IWalkableWorld.h"

USING_NS_CC;

GroundMoveComponent::GroundMoveComponent()
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
    if (toTarget.lengthSquared() < 0.0001f)
        return;

    Vec2 moveDir = toTarget.getNormalized();
    float speed = owner->getMoveSpeed();
    Vec2 desiredPos = ownerPos + moveDir * speed * dt;
    Vec2 finalPos = world->constrainPosition(desiredPos, ownerPos);

    // 如果前方不可走（遇到河），尝试找桥
    if (!world->canWalk(desiredPos))
    {
		CCLOG("Encountered obstacle, attempting to find bridge.");
        // 1. 寻找最近桥的 X 坐标
        float bridgeX = world->getNearestBridgeX(ownerPos);//桥的中心坐标

        // 2. 决定滑行方向（向桥滑行）
        float diffX = bridgeX - ownerPos.x;
        float dirX = (diffX > 0) ? 1.f : -1.f;
        

        if (std::abs(diffX) > 25.0f) //这样做是为了判定我在桥的外界，但是我有可能进入桥了
        {
            _riverDir = Vec2(dirX, 0.f);
            
            // 尝试沿河滑行
            Vec2 slidePos = ownerPos + _riverDir * speed * dt;
            
            if (world->canWalk(slidePos))
            {
				CCLOG("Sliding along river towards bridge at X=%.2f and now we at %f %f", bridgeX,slidePos.x, slidePos.y);
                finalPos = slidePos;
            }
            else
            {
                // 如果滑行也被挡，尝试稍微远离河岸一点点再滑
                Vec2 awayFromRiver = -moveDir;
                Vec2 backupPos = ownerPos + (awayFromRiver * 0.5f + _riverDir) * speed * dt;
                 if (world->canWalk(backupPos))
                     finalPos = backupPos;
                 else
                     finalPos = ownerPos;
            }
        }
        else
        {
            // 已对准桥口
            // 强制构造一个指向桥内（沿 Y 轴）的向量
            Vec2 bridgeDir = (targetPos.y > ownerPos.y) ? Vec2(0, 1) : Vec2(0, -1);
            
            // 弱修正 X，使其趋向桥中心
            bridgeDir.x = diffX * 2.0f / speed; // 动态调整修正力度
            bridgeDir.normalize();

            // 如果已经非常接近桥中心（X差值极小），可以加大纵向移动的权重，防止一直微调 X
            if (std::abs(diffX) < 5.0f)
            {
                 bridgeDir.y *= 2.0f; // 强化纵向意图
                 bridgeDir.normalize();
            }

            Vec2 bridgeEnterPos = ownerPos + bridgeDir * speed * dt;
            
            // 尝试进入
            finalPos = world->constrainPosition(bridgeEnterPos, ownerPos);
			CCLOG("%f %f Attempting to enter bridge towards %f %f", ownerPos.x, ownerPos.y, bridgeEnterPos.x, bridgeEnterPos.y);
            // 如果依然不可走（比如卡在判定边缘），强制 X 对齐
            if (finalPos == ownerPos)
            {
                // 直接移动到桥中心 X
                float stepX = (diffX > 0 ? 1 : -1) * std::min(std::abs(diffX), speed * dt);
                finalPos.x += stepX;
                
                // 再次尝试纵向推进
                Vec2 pureYPos = finalPos + Vec2(0, (targetPos.y > ownerPos.y ? 1 : -1)) * speed * dt;
                
                // 只有当 pureYPos 合法时才采纳纵向移动，否则只修正 X
                // 关键修正：如果 pureYPos 判定为 bridge，我们也应该放行！
                // 之前的逻辑只看了 canWalk，可能对 bridge 判定不完全
                if (world->canWalk(pureYPos) || world->isBridge(pureYPos))
                {
                    finalPos = pureYPos;
                }
                else
                {
                     // 最后的手段：如果纵向依然被挡，强行往桥中心（X轴）再挤一挤
                     // 有时候是因为判定点刚好在 (Grid边界 - epsilon) 的位置
                     // 强制给一个微小的纵向位移尝试“突围”
                     Vec2 forceEnter = pureYPos + Vec2(0, (targetPos.y > ownerPos.y ? 1 : -1)) * 2.0f;
                     if (world->isBridge(forceEnter))
                     {
                         finalPos = forceEnter;
                     }
                }
            }
        }
    }

    
    // 如果已经在桥上，确保平滑移动
    // 注意：constrainPosition 会处理桥上的移动，所以我们这里可以信任它
    // 但是要小心，如果 desiredPos 已经偏离桥太多，constrainPosition 可能会把它拉回来
    // 这里的逻辑主要是为了保证在桥上的移动是连续的
    if (world->isBridge(ownerPos))
    {
        finalPos = world->constrainPosition(desiredPos, ownerPos);
    }

    Node* scene = owner->getParent();
    if (scene)
    {
        for (Node* node : scene->getChildren())
        {
            auto other = dynamic_cast<IAttackable*>(node);
            if (!other || other == owner || other == _followTarget || other->isDead())
                continue;
            if (owner->getMoveType() != other->getMoveType())
                continue;

            Vec2 diff = finalPos - node->getPosition();
            float dist = diff.length();
            float minDist = owner->getBodyRadius() + other->getBodyRadius();

            if (dist < minDist && dist > 0.001f)
            {
                Vec2 pushDir = diff / dist;
                finalPos = node->getPosition() + pushDir * minDist;
                break;
            }
        }
    }

    owner->setPosition(finalPos);
}
