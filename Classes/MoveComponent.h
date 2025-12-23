#ifndef __MOVE_COMPONENT_H__
#define __MOVE_COMPONENT_H__

#include "cocos2d.h"

// 前向声明
class TroopBase;
class IAttackable;

USING_NS_CC;

// =========================
// MoveComponent
// =========================
//
// 移动组件抽象基类（支持攻击任何IAttackable目标）
// - 保存通用移动状态
// - 提供模板方法 update
//
class MoveComponent
{
public:
    MoveComponent();
    virtual ~MoveComponent();

    // 高层移动意图（AI 调用）
    virtual void followTarget(IAttackable* target, float desiredDistance);
    virtual void stop();

    // 每帧更新（TroopBase 调用）
    virtual void update(TroopBase* owner, float dt);

    // 获取当前目标
    IAttackable* getFollowTarget() const { return _followTarget; }

    // 获取期望距离
    float getDesiredDistance() const { return _desiredDistance; }

    // 是否正在移动
    bool isMoving() const { return _moving; }

protected:
    // =========================
    // 通用移动状态（关键）
    // =========================
    IAttackable* _followTarget = nullptr;  // 当前跟随的目标
    float _desiredDistance = 0.0f;         // 希望保持的距离（攻击距离）
    bool _moving = false;                  // 是否正在移动

protected:
    // =========================
    // 子类实现的具体移动逻辑
    // =========================
    virtual void onUpdateMove(TroopBase* owner, float dt) = 0;
};

#endif // __MOVE_COMPONENT_H__