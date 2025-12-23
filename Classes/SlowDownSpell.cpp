// SlowDownSpell.cpp
#include "SlowDownSpell.h"
#include "cocos2d.h"

USING_NS_CC;

SlowDownSpell::SlowDownSpell()
{
}

bool SlowDownSpell::init()
{
    if (!SpellBase::init())
        return false;

    // 调试绘制
    _debugDraw = DrawNode::create();
    addChild(_debugDraw, 10);

    return true;
}

void SlowDownSpell::cast(const Vec2& worldPos, ECamp casterCamp)
{
    if (_hasCast)
        return;

    _hasCast = true;
    _casterCamp = casterCamp;

    setPosition(worldPos);

    CCLOG("SlowDownSpell cast at (%.1f, %.1f)", worldPos.x, worldPos.y);

    drawDebugRange();
    applySlow();

    // 短暂存在后移除
    runAction(Sequence::create(
        DelayTime::create(0.2f),
        RemoveSelf::create(),
        nullptr
    ));
}

void SlowDownSpell::applySlow()
{
    Node* parent = getParent();
    if (!parent)
        return;

    // 法术中心的世界坐标
    Vec2 center = this->convertToWorldSpaceAR(Vec2::ZERO);

    for (Node* node : parent->getChildren())
    {
        auto target = dynamic_cast<IAttackable*>(node);
        if (!target)
            continue;

        if (target->isDead())
            continue;

        // 同阵营不受影响
        if (target->getCamp() == _casterCamp)
            continue;

        float dist = center.distance(target->getWorldPosition());
        float hitRange = _radius + target->getBodyRadius();

        if (dist <= hitRange)
        {
            target->applySlow(_slowRatio, _duration);
            CCLOG("SlowDownSpell apply slow to %p (ratio=%.2f, duration=%.1f)",
                target, _slowRatio, _duration);
        }
    }
}

void SlowDownSpell::drawDebugRange()
{
    if (!_debugDraw)
        return;

    _debugDraw->clear();

    // 用蓝色表示减速区域，和火球区分
    _debugDraw->drawCircle(
        Vec2::ZERO,
        _radius,
        0,
        40,
        false,
        Color4F::BLUE
    );
}
