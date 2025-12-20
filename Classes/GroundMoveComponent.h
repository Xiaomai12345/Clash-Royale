#ifndef __GROUND_MOVE_COMPONENT_H__
#define __GROUND_MOVE_COMPONENT_H__

#include "MoveComponent.h"

// =========================
// GroundMoveComponent
// =========================
//
// 地面移动组件（直线追踪版本）
// 支持追踪：士兵(TroopBase) 和 建筑(BuildingBase)
//
class GroundMoveComponent : public MoveComponent
{
public:
    GroundMoveComponent();
    virtual ~GroundMoveComponent() = default;

protected:
    // 具体的移动逻辑实现
    virtual void onUpdateMove(TroopBase* owner, float dt) override;

private:
    // 辅助方法：计算实际需要移动的距离
    float calculateMovement(TroopBase* owner, const cocos2d::Vec2& targetPos);
};

#endif // __GROUND_MOVE_COMPONENT_H__