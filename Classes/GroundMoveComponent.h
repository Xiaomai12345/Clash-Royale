#ifndef __GROUND_MOVE_COMPONENT_H__
#define __GROUND_MOVE_COMPONENT_H__

#include "MoveComponent.h"
#include "cocos2d.h"

class GroundMoveComponent : public MoveComponent
{
public:
    GroundMoveComponent();
    virtual ~GroundMoveComponent() = default;

protected:
    virtual void onUpdateMove(TroopBase* owner, float dt) override;

private:
    // =====================
    // 绕行（避障）状态
    // =====================
    bool _isOrbiting = false;        // 是否正在绕行
    cocos2d::Vec2 _orbitDir;         //  锁死的绕行方向
    float _orbitTimer = 0.f;         // 绕行最小持续时间

    // 参数（可调）
    float _minOrbitTime = 0.25f;     // 最短绕行时间
};

#endif // __GROUND_MOVE_COMPONENT_H__
