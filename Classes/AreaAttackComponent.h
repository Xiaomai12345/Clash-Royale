#ifndef __AREA_ATTACK_COMPONENT_H__
#define __AREA_ATTACK_COMPONENT_H__

#include "AttackComponent.h"
#include "cocos2d.h"

class AreaAttackComponent : public AttackComponent
{
public:
    AreaAttackComponent(float range, float interval, int damage, float aoeRadius);

protected:
    virtual void doAttack(IAttackable* owner) override;

private:
    int   _damage = 0;
    float _aoeRadius = 0.f;

    // AOE 可视化节点
    cocos2d::DrawNode* _aoeDraw = nullptr;
};

#endif // __AREA_ATTACK_COMPONENT_H__
