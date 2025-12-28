#ifndef __BUILDING_AI_COMPONENT_H__
#define __BUILDING_AI_COMPONENT_H__

#include "BuildingBase.h"


class BuildingAI
{
public:
    BuildingAI();
    virtual ~BuildingAI() = default;

    virtual void update(BuildingBase* owner, float dt) = 0;  


    void setTarget(IAttackable* target) { _target = target; }

    IAttackable* getTarget() const { return _target; }


    void clearTarget() { _target = nullptr; }

    bool hasTarget() const { return _target != nullptr && !_target->isDead(); }

protected:
    IAttackable* _target = nullptr; 
};

#endif // __BUILDING_AI_COMPONENT_H__