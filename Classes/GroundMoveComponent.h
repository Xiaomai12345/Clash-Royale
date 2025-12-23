#ifndef __GROUND_MOVE_COMPONENT_H__
#define __GROUND_MOVE_COMPONENT_H__

#include "MoveComponent.h"
#include "cocos2d.h"
class IWalkableWorld;
class GroundMoveComponent : public MoveComponent
{
public:
    GroundMoveComponent();
    virtual ~GroundMoveComponent() = default;

protected:
    virtual void onUpdateMove(TroopBase* owner, float dt) override;

private:
    // ===== 河岸滑行 =====
    bool _isSlidingRiver = false;
    float _riverLockTimer = 0.f;
    float _riverLockTime = 0.4f;
    cocos2d::Vec2 _riverDir;
    cocos2d::Vec2 _lastValidPos;

    // ===== 桥状态 =====
    bool _isOnBridge = false;

    // ===== 单位碰撞绕行 =====
    bool _isOrbiting = false;
    cocos2d::Vec2 _orbitDir;
    float _orbitTimer = 0.f;
    float _minOrbitTime = 0.2f;

private:
    bool findRiverExit(
        const cocos2d::Vec2& start,
        float dirX,
        IWalkableWorld* world,
        cocos2d::Vec2& outExit);
};

#endif
