#include "MeleeAreaAttackComponent.h"
#include "TroopBase.h"
#include "BuildingBase.h"
#include "cocos2d.h"

USING_NS_CC;

MeleeAreaAttackComponent::MeleeAreaAttackComponent(
    float range,
    float interval,
    int damage)
    : AttackComponent(range, interval)
    , _damage(damage)
{
}

void MeleeAreaAttackComponent::doAttack(IAttackable* owner)
{

    auto ownerNode = dynamic_cast<IAttackable*>(owner);
    if (!ownerNode)
        return;

    Node* parent = ownerNode->getParent();
    if (!parent)
        return;

    Vec2 center = ownerNode->getWorldPosition();
    float attackRange = getAttackRange();

    // =========================
    // 1?? 显示攻击范围闪烁
    // =========================
    showAttackRange(ownerNode);

    // =========================
    // 2?? 对范围内敌人造成伤害
    // =========================
    for (Node* node : parent->getChildren())
    {
        auto target = dynamic_cast<IAttackable*>(node);
        if (!target || target->isDead())
            continue;

        if (target == owner)
            continue;

        if (target->getCamp() == owner->getCamp())
            continue;

        float dist = target->getWorldPosition().distance(center);
        float effectiveRange = attackRange + target->getBodyRadius();

        if (dist <= effectiveRange)
        {
            target->takeDamage(_damage);

            CCLOG("MeleeAOE: %p hit %p for %d damage",
                owner, target, _damage);
        }
    }
}

void MeleeAreaAttackComponent::showAttackRange(Node* ownerNode)
{
    if (!_rangeDraw)
    {
        _rangeDraw = DrawNode::create();
        ownerNode->addChild(_rangeDraw, 999);
    }

    _rangeDraw->clear();

    float r = getAttackRange();

    // 画攻击范围圆（本地坐标，圆心在自己脚下）
    _rangeDraw->drawCircle(
        Vec2::ZERO,
        r,
        0,
        32,
        false,
        Color4F(1.f, 0.5f, 0.2f, 0.8f) // 女武神橙红色
    );

    _rangeDraw->setOpacity(255);
    _rangeDraw->setScale(0.8f);

    // 闪烁动画：放大 + 淡出
    _rangeDraw->runAction(Sequence::create(
        Spawn::create(
            ScaleTo::create(0.15f, 1.0f),
            FadeOut::create(0.15f),
            nullptr
        ),
        CallFunc::create([this]() {
            if (_rangeDraw)
                _rangeDraw->clear();
            }),
        nullptr
    ));
}
