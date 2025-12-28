#ifndef __SIMPLE_BUILDING_AI_COMPONENT_H__
#define __SIMPLE_BUILDING_AI_COMPONENT_H__

#include "BuildingAI.h"
#include"BuildingBase.h"

class SimpleBuildingAI : public BuildingAI
{
public:
    SimpleBuildingAI(float detectionRange = 400.0f);

    virtual void update(BuildingBase* owner, float dt) override;


    void setDetectionRange(float range) { _detectionRange = range; }

    float getDetectionRange() const { return _detectionRange; }

private:

    IAttackable* findEnemyInRange(IAttackable* owner);

    bool isValidTarget(BuildingBase* owner, IAttackable* target) const;

    float calculateDistance(BuildingBase* owner, IAttackable* target) const;

private:
    float _detectionRange;   // 检测范围（可能大于攻击范围）
    float _searchCooldown;   // 搜索冷却计时器
    static const float SEARCH_INTERVAL; // 搜索间隔时间
};

#endif // __SIMPLE_BUILDING_AI_COMPONENT_H__