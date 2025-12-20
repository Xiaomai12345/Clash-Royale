#ifndef __SIMPLE_BUILDING_AI_COMPONENT_H__
#define __SIMPLE_BUILDING_AI_COMPONENT_H__

#include "BuildingAI.h"
#include"BuildingBase.h"
/**
 * 简单建筑AI组件
 * 功能：自动寻找并攻击范围内的敌人
 */
class SimpleBuildingAI : public BuildingAI
{
public:
    SimpleBuildingAI(float detectionRange = 400.0f);

    /**
     * 重写更新方法
     */
    virtual void update(BuildingBase* owner, float dt) override;

    /**
     * 设置检测范围
     * @param range 检测范围
     */
    void setDetectionRange(float range) { _detectionRange = range; }

    /**
     * 获取检测范围
     * @return 检测范围
     */
    float getDetectionRange() const { return _detectionRange; }

private:
    /**
     * 寻找范围内的敌人
     * @param owner 建筑自身
     * @return 找到的敌人，未找到返回nullptr
     */
    IAttackable* findEnemyInRange(BuildingBase* owner);

    /**
     * 检查目标是否有效且在范围内
     * @param owner 建筑自身
     * @param target 检查的目标
     * @return 是否有效且在范围内
     */
    bool isValidTarget(BuildingBase* owner, IAttackable* target) const;

    /**
     * 计算与目标的距离
     * @param owner 建筑自身
     * @param target 目标
     * @return 距离
     */
    float calculateDistance(BuildingBase* owner, IAttackable* target) const;

private:
    float _detectionRange;   // 检测范围（可能大于攻击范围）
    float _searchCooldown;   // 搜索冷却计时器
    static const float SEARCH_INTERVAL; // 搜索间隔时间
};

#endif // __SIMPLE_BUILDING_AI_COMPONENT_H__