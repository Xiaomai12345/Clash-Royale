#ifndef __MELEE_AREA_ATTACK_COMPONENT_H__
#define __MELEE_AREA_ATTACK_COMPONENT_H__

#include "AttackComponent.h"
#include "cocos2d.h"

// MeleeAreaAttackComponent
// ------------------------
// 近战范围攻击（女武神）
// 攻击范围 = AOE 范围
//
class MeleeAreaAttackComponent : public AttackComponent
{
public:
    MeleeAreaAttackComponent(float range, float interval, int damage);

protected:
    virtual void doAttack(IAttackable* owner) override;

private:
    int _damage = 0;

    // 攻击范围可视化
    cocos2d::DrawNode* _rangeDraw = nullptr;

    void showAttackRange(cocos2d::Node* ownerNode);
};

#endif // __MELEE_AREA_ATTACK_COMPONENT_H__
