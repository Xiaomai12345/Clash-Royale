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


    // 获取子节点副本，防止遍历时删除节点导致迭代器失效
    auto children = parent->getChildren();
    
    // 使用倒序索引遍历，防止迭代器失效
    for (ssize_t i = children.size() - 1; i >= 0; --i)
    {
        Node* node = children.at(i);
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


    ownerNode->runAction(Sequence::create(
        DelayTime::create(0.3f),  // AOE 显示 0.3 秒后消失
        CallFunc::create([this]() {
            if (_aoeDraw)
                _aoeDraw->clear();  // 清除 AOE 范围
            }),
        nullptr
    ));
}

