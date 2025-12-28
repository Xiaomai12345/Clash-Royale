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

    // 1. 创建火球精灵 (起始位置在屏幕上方)
    auto fireballSprite = Sprite::create("Images/cards/fireball(1).png"); // 暂时用卡牌图标代替，建议换成特效图
    if (fireballSprite)
    {
        addChild(fireballSprite);
        fireballSprite->setScale(0.5f);
        
        // 初始位置：目标点上方很高的地方
        Vec2 startPos = Vec2(0, 400); 
        fireballSprite->setPosition(startPos);
        
        // 2. 投掷动画：移动到目标点 (0,0)
        float duration = 0.5f;
        auto move = MoveTo::create(duration, Vec2::ZERO);
        auto rotate = RotateBy::create(duration, 360); // 旋转效果
        auto spawn = Spawn::create(move, rotate, nullptr);
        
        // 3. 落地后的回调
        auto callback = CallFunc::create([this, fireballSprite]() {
            // 隐藏火球
            fireballSprite->setVisible(false);
            
            // 造成伤害
            applyDamage();
            
            // 播放爆炸特效 (这里可以用粒子系统或帧动画，暂时用简单的缩放模拟)
            auto explosion = Sprite::create("Images/cards/fireball(1).png"); // 临时用
            if (explosion) {
                this->addChild(explosion);
                explosion->setScale(0.1f);
                explosion->runAction(Sequence::create(
                    ScaleTo::create(0.1f, 1.5f), // 快速膨胀
                    FadeOut::create(0.2f),       // 渐隐消失
                    RemoveSelf::create(),
                    nullptr
                ));
            }
            
            // 绘制调试范围
            drawDebugRange();
        });

        // 4. 移除自身
        // 修正：等待飞行时间 (0.5s) + 爆炸动画时间 (0.3s) + 一点缓冲
        auto remove = Sequence::create(DelayTime::create(0.5f + 0.3f + 0.1f), RemoveSelf::create(), nullptr);

        fireballSprite->runAction(Sequence::create(spawn, callback, nullptr));
        
        // 注意：这里不要立即 RemoveSelf，要等动画播放完
        runAction(remove);
    }
    else
    {
        // 如果没有图片，直接结算
        drawDebugRange();
        applyDamage();
        runAction(Sequence::create(DelayTime::create(0.5f), RemoveSelf::create(), nullptr));
    }
}

void FireballSpell::applyDamage()
{
    CCLOG("FireBall has been applied");
    Node* parent = getParent();
    if (!parent)
        return;

    // Fireball 的世界坐标
    Vec2 center = this->convertToWorldSpaceAR(Vec2::ZERO);

    // 获取 BattleField 的子节点
    // 注意：获取子节点副本，防止遍历时删除节点导致迭代器失效
    auto children = parent->getChildren();
    
    // 使用倒序索引遍历，防止迭代器失效
    for (ssize_t i = children.size() - 1; i >= 0; --i)
    {
        Node* node = children.at(i);
        // 先判断是否是 IAttackable
        auto target = dynamic_cast<IAttackable*>(node);
        if (!target)
            continue;

        if (target->isDead())
            continue;

        // 判断目标阵营是否与施法阵营不同
        if (target->getCamp() == _casterCamp)  // 目标与法术同阵营
            continue;

        // 计算距离
        float dist = center.distance(target->getWorldPosition());
        float hitRange = _radius + target->getBodyRadius();

        if (dist <= hitRange)
        {
            // 对建筑和士兵分别处理
            // 使用 target 进行 cast，而不是 node (虽然在这个 context 下是一样的，但更严谨)
            if (dynamic_cast<TroopBase*>(target))
            {
                target->takeDamage(_damage);
                CCLOG("Fireball hit %p Troop for %d damage", target, _damage);
            }
            else if (dynamic_cast<BuildingBase*>(target))
            {
                // 建筑受到较少伤害 (例如 30%)
                int buildingDamage = static_cast<int>(_damage * 0.3f); 
                target->takeDamage(buildingDamage);
                CCLOG("Fireball hit %p Building for %d damage", target, buildingDamage);
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
