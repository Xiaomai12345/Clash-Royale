// SlowDownSpell.h
#ifndef __SLOW_DOWN_SPELL_H__
#define __SLOW_DOWN_SPELL_H__

#include "SpellBase.h"
#include "IAttackable.h"

class SlowDownSpell : public SpellBase
{
public:
    SlowDownSpell();
    CREATE_FUNC(SlowDownSpell);

    virtual bool init() override;

    // 施放减速法术
    virtual void cast(const Vec2& worldPos, ECamp casterCamp) override;

protected:
    // 查找并施加减速效果
    void applySlow();

    // 调试显示减速范围
    void drawDebugRange();

protected:
    float _radius = 120.0f;      // 影响半径
    float _slowRatio = 0.5f;     // 减速比例（0.5 = 50% 速度）
    float _duration = 2.0f;      // 持续时间（秒）

    DrawNode* _debugDraw = nullptr;
};

#endif // __SLOW_DOWN_SPELL_H__
