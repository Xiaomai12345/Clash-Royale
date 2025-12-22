#ifndef __TROOP_AI_COMPONENT_H__
#define __TROOP_AI_COMPONENT_H__

class IAttackable;
class TroopBase;
class TroopAIComponent
{
public:
    virtual ~TroopAIComponent() = default;

    // AI 每帧决策（只下命令）
    virtual void update(TroopBase* owner, float dt) = 0;

    void setTarget(IAttackable* target) { _target = target; }
protected:
    IAttackable* _target = nullptr;
};

#endif
