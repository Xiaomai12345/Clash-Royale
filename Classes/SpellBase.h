// SpellBase.h
#ifndef __SPELL_BASE_H__
#define __SPELL_BASE_H__

#include "cocos2d.h"
#include"IAttackable.h"
USING_NS_CC;

// =========================
// SpellBase
// =========================
//
// 法术基类（一次性效果）
// - 由 Card 创建并施放
// - 不可被攻击
// - 不参与 AI / Move / Attack
//
class SpellBase : public Node
{
public:
    SpellBase();
    virtual ~SpellBase();

    virtual bool init() override;

    // 施放法术（核心入口）
    // worldPos : 世界坐标
    // casterCamp : 施法者阵营（LEFT / RIGHT）
    virtual void cast(const Vec2& worldPos, ECamp casterCamp) = 0;

protected:
    // 法术所属阵营
    ECamp _casterCamp = ECamp::LEFT;//默认阵营

    // 是否已经生效（防止重复触发）
    bool _hasCast = false;
};

#endif // __SPELL_BASE_H__
