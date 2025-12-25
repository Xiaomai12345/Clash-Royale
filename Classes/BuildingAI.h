#ifndef __BUILDING_AI_COMPONENT_H__
#define __BUILDING_AI_COMPONENT_H__

#include "BuildingBase.h"

/**
 * 建筑AI基类
 * 与TroopAIComponent对应，负责建筑的AI行为
 */
class BuildingAI
{
public:
    BuildingAI();
    virtual ~BuildingAI() = default;

    /**
     * 每帧更新AI逻辑
     * @param owner 拥有此AI的建筑
     * @param dt 时间增量
     */
    virtual void update(BuildingBase* owner, float dt) = 0;  // ✅ 修正：参数类型

    /**
     * 设置攻击目标
     * @param target 攻击目标
     */
    void setTarget(IAttackable* target) { _target = target; }

    /**
     * 获取当前目标
     * @return 当前攻击目标
     */
    IAttackable* getTarget() const { return _target; }

    /**
     * 清除目标
     */
    void clearTarget() { _target = nullptr; }

    /**
     * 是否有目标
     * @return 是否有有效目标
     */
    bool hasTarget() const { return _target != nullptr && !_target->isDead(); }

protected:
    IAttackable* _target = nullptr;  // 当前攻击目标
};

#endif // __BUILDING_AI_COMPONENT_H__