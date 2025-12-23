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
    // ===== 桥状态管理 =====
    enum class BridgeState
    {
        None,
        ToBridge,
        OnBridge,
        Passed
    };
    BridgeState _bridgeState = BridgeState::None;
    cocos2d::Vec2 _cachedBridgePos;

    // ===== 河岸滑行 =====
    cocos2d::Vec2 _riverDir;
};

#endif
