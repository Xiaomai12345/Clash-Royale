#include "AreaAttackComponent.h"
#include "TroopBase.h"
#include "BuildingBase.h"

USING_NS_CC;

AreaAttackComponent::AreaAttackComponent(
    float range,
    float interval,
    int damage,
    float aoeRadius)
    : AttackComponent(range, interval)
    , _damage(damage)
    , _aoeRadius(aoeRadius)
{
}
void AreaAttackComponent::doAttack(IAttackable* owner)
{
    auto ownerNode = dynamic_cast<Node*>(owner);
    if (!ownerNode)
        return;

    if (!_target || _target->isDead())
        return;

    Node* parent = ownerNode->getParent();
    if (!parent)
        return;

    Vec2 aoeCenterWorld = _target->getWorldPosition(); // 获取目标位置

    // =========================
    // 显示 AOE 范围
    // =========================
    if (!_aoeDraw)
    {
        _aoeDraw = DrawNode::create();
        ownerNode->addChild(_aoeDraw, 999);  // 确保它绘制在上层
    }

    _aoeDraw->clear();  // 清空之前的绘制

    // 世界坐标 → owner 本地坐标
    Vec2 localCenter = ownerNode->convertToNodeSpace(aoeCenterWorld);

    _aoeDraw->drawCircle(
        localCenter,        // 圆心
        _aoeRadius,         // 半径
        0,                  // 圆周的细分数，越大越平滑
        32,                 // 圆的细分数
        false,              // 是否填充
        Color4F(1, 0, 0, 0.8f)  // 圆的颜色（红色）
    );

    // =========================
    // 造成范围伤害
    // =========================
    for (Node* node : parent->getChildren())
    {
        auto target = dynamic_cast<IAttackable*>(node);
        if (!target || target->isDead())
            continue;

        if (target->getCamp() == owner->getCamp())
            continue;

        float distance = target->getWorldPosition().distance(aoeCenterWorld);

        if (distance <= _aoeRadius)
        {
            target->takeDamage(_damage);

            CCLOG("AreaAttack: %p hit %p for %d damage",
                owner, target, _damage);
        }
    }

    // =========================
    // 4️⃣ 延迟隐藏 AOE（否则看不见）
    // =========================
    ownerNode->runAction(Sequence::create(
        DelayTime::create(0.3f),  // AOE 显示 0.3 秒后消失
        CallFunc::create([this]() {
            if (_aoeDraw)
                _aoeDraw->clear();  // 清除 AOE 范围
            }),
        nullptr
    ));
}

