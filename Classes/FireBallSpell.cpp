// FireballSpell.cpp
#include "FireballSpell.h"
#include "IAttackable.h"
#include"TroopBase.h"
#include"BuildingBase.h"
#include "cocos2d.h"

USING_NS_CC;

FireballSpell::FireballSpell()
{
}

bool FireballSpell::init()
{
    if (!SpellBase::init())
        return false;

    // 调试绘制
    _debugDraw = DrawNode::create();
    addChild(_debugDraw, 10);

    return true;
}

void FireballSpell::cast(const Vec2& worldPos, ECamp casterCamp)
{
    if (_hasCast)
        return;

    _hasCast = true;
    _casterCamp = casterCamp;

    // Spell 本身是 Node，用 Node 的方式
    setPosition(worldPos);

    CCLOG("Fireball cast at (%.1f, %.1f)", worldPos.x, worldPos.y);

    drawDebugRange();
    applyDamage();

    // 延迟移除，方便观察
    runAction(Sequence::create(
        DelayTime::create(0.2f),
        RemoveSelf::create(),
        nullptr
    ));
}

void FireballSpell::applyDamage()
{
    CCLOG("FireBall has benn applyed");
    Node* parent = getParent();
    if (!parent)
        return;
    CCLOG("FireBall has benn applyedd");
    // Spell 的世界坐标（⚠️ 正确方式）
    Vec2 center = this->convertToWorldSpaceAR(Vec2::ZERO);

    for (Node* node : parent->getChildren())
    {
        auto target = dynamic_cast<IAttackable*>(node);
        if (!target)
            continue;

        if (target->isDead())
            continue;

        if (target->getCamp() == _casterCamp)//目标与法术同阵营
            continue;

        float dist = center.distance(target->getWorldPosition());
        float hitRange = _radius + target->getBodyRadius();

        if (dist <= hitRange)
        {
            //对建筑还有士兵进行分别讨论
            if (target == dynamic_cast<TroopBase*>(node))
            {
                target->takeDamage(_damage);
                CCLOG("Fireball hit %p Troop for %d damage", target, _damage);
            }
            else if(target == dynamic_cast<BuildingBase*>(node))
            {
                target->takeDamage(0.3*_damage);
                CCLOG("Fireball hit %p Troop for %d damage", target, _damage);
            }
        }
    }
}

void FireballSpell::drawDebugRange()
{
    if (!_debugDraw)
        return;
    CCLOG("FireBall has benn debuged");
    _debugDraw->clear();
    _debugDraw->drawCircle(
        Vec2::ZERO,
        _radius,
        0,
        40,
        false,
        Color4F::RED
    );
}
