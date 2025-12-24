#include "FireballSpell.h"
#include "IAttackable.h"
#include "TroopBase.h"
#include "BuildingBase.h"
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
    _casterCamp = casterCamp; // 保存施法者阵营信息

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
    CCLOG("FireBall has been applied");
    Node* parent = getParent();
    if (!parent)
        return;

    // Fireball 的世界坐标
    Vec2 center = this->convertToWorldSpaceAR(Vec2::ZERO);

    for (Node* node : parent->getChildren())
    {
        auto target = dynamic_cast<IAttackable*>(node);
        if (!target)
            continue;

        if (target->isDead())
            continue;

        // 判断目标阵营是否与施法阵营不同
        if (target->getCamp() == _casterCamp)  // 目标与法术同阵营
            continue;

        float dist = center.distance(target->getWorldPosition());
        float hitRange = _radius + target->getBodyRadius();

        if (dist <= hitRange)
        {
            // 对建筑和士兵分别处理
            if (target == dynamic_cast<TroopBase*>(node))
            {
                target->takeDamage(_damage);
                CCLOG("Fireball hit %p Troop for %d damage", target, _damage);
            }
            else if (target == dynamic_cast<BuildingBase*>(node))
            {
                target->takeDamage(0.3 * _damage);  // 对建筑造成30%伤害
                CCLOG("Fireball hit %p Building for %d damage", target, 0.3 * _damage);
            }
        }
    }
}

void FireballSpell::drawDebugRange()
{
    if (!_debugDraw)
        return;

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
