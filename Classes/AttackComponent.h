#ifndef __ATTACK_COMPONENT_H__
#define __ATTACK_COMPONENT_H__

#include "cocos2d.h"
#include "IAttackable.h"

USING_NS_CC;

// =========================
// AttackComponent（通用攻击组件）
// =========================
//
// 职责：
// 1. 管理攻击目标
// 2. 判断攻击范围
// 3. 控制攻击冷却
// 4. 触发具体攻击行为
//
class AttackComponent
{
public:
    AttackComponent(float range, float interval);
    virtual ~AttackComponent() = default;

    // 设置目标
    void setTarget(IAttackable* target);
    void clearTarget();

    bool hasTarget() const { return _target != nullptr; }

    // 每帧更新（由拥有者调用）
    void update(IAttackable* owner, float dt);

    // 攻击范围
    float getAttackRange() const { return _attackRange; }

    // ====== 修正：将 isTargetInRange 移到 public ======
    bool isTargetInRange(IAttackable* owner) const;

protected:
    // 真正的攻击行为（子类实现）
    virtual void doAttack(IAttackable* owner) = 0;

protected:
    IAttackable* _target = nullptr;

	float _attackRange = 0.f;// 攻击范围
	float _attackInterval = 1.f;// 攻击间隔（秒）
	float _cooldown = 0.f;// 攻击冷却计时器
};

#endif // __ATTACK_COMPONENT_H__
