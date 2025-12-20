// FireballSpell.h
#ifndef __FIREBALL_SPELL_H__
#define __FIREBALL_SPELL_H__

#include "SpellBase.h"
#include "IAttackable.h"

class FireballSpell : public SpellBase
{
public:
    FireballSpell();
    CREATE_FUNC(FireballSpell);

    virtual bool init() override;

    // 施放火球
    virtual void cast(const Vec2& worldPos, ECamp casterCamp) override;

protected:
    // 查找并伤害目标
    void applyDamage();

    // 调试显示攻击范围
    void drawDebugRange();

protected:
    float _radius = 100.0f;   // 爆炸半径
    int   _damage = 150;      // 伤害值

    DrawNode* _debugDraw = nullptr;
};

#endif // __FIREBALL_SPELL_H__
