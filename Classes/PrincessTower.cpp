#include "PrincessTower.h"
#include "BuildingAttackComponent.h"
#include "cocos2d.h"
#include"SimpleBuildingAI.h"
USING_NS_CC;

bool PrincessTower::init()
{
    // 1. 调用父类初始化
    if (!BuildingBase::init())
        return false;

    // 2. 设置基础属性
    _maxHp = 1200;
    _hp = _maxHp;
    _bodyRadius = 25.0f;

    // 3. 初始化组件
    auto ai = new SimpleBuildingAI();
    setAIComponent(ai);

    setupAttack();
    setupAppearance();

    CCLOG("公主塔初始化完成，位置：(%.0f, %.0f)", getPositionX(), getPositionY());

    return true;
}

void PrincessTower::setupAttack()
{
    // 创建建筑攻击组件
    auto attack = new BuildingAttackComponent(
        _attackRange,    // 攻击范围
        _attackInterval, // 攻击间隔
        _attackDamage    // 伤害值
    );

    setAttackComponent(attack);
    CCLOG("公主塔攻击组件：范围=%.0f，间隔=%.1f，伤害=%d",
        _attackRange, _attackInterval, _attackDamage);
}

void PrincessTower::setupAppearance()
{
    // 创建塔身
    _sprite = Sprite::create("Images/towers/princess_tower_red.png");
    if (_sprite)
    {
        addChild(_sprite);
        _sprite->setScale(1.0f);

        // 简单装饰
        auto flag = Sprite::create("tower_flag.png");
        if (flag)
        {
            flag->setPosition(Vec2(0, 50));
            flag->setScale(0.3f);
            _sprite->addChild(flag);
        }
        else
        {
            // 没有图片就用绘制节点
            auto draw = DrawNode::create();
            draw->drawSolidRect(Vec2(-5, 45), Vec2(5, 60), Color4F::RED);
            _sprite->addChild(draw);
        }
    }
    else
    {
        // 加载失败就用默认图形
        _sprite = Sprite::create();
        addChild(_sprite);

        auto draw = DrawNode::create();
        // 塔身
        draw->drawSolidRect(Vec2(-25, -25), Vec2(25, 75), Color4F(0.8f, 0.6f, 0.4f, 1.0f));
        // 塔顶
        draw->drawSolidCircle(Vec2(0, 95), 20, 0, 12, Color4F(0.9f, 0.3f, 0.3f, 1.0f));
        _sprite->addChild(draw);

        CCLOG("公主塔：使用默认图形");
    }
}

void PrincessTower::update(float dt)
{
    // 调用父类更新（会处理攻击逻辑）
    BuildingBase::update(dt);
}

void PrincessTower::takeDamage(int damage)
{
    // 先调用父类处理伤害
    BuildingBase::takeDamage(damage);

    // 简单受伤效果
    if (_sprite && damage > 0)
    {
        // 闪烁一下
        auto tintRed = TintTo::create(0.1f, Color3B::RED);
        auto tintBack = TintTo::create(0.1f, _sprite->getColor());
        _sprite->runAction(Sequence::create(tintRed, tintBack, nullptr));
    }

    // 显示伤害数字
    showDamageNumber(damage);

    CCLOG("公主塔受到 %d 伤害，剩余血量：%d/%d", damage, _hp, _maxHp);
}

void PrincessTower::showDamageNumber(int damage)
{
    auto label = Label::createWithTTF(
        StringUtils::format("-%d", damage),
        "fonts/arial.ttf",
        18
    );
    if (!label) return;

    label->setColor(Color3B::RED);
    label->setPosition(Vec2(0, 100));
    addChild(label, 100);

    // 简单的上浮消失动画
    auto move = MoveBy::create(0.6f, Vec2(0, 40));
    auto fade = FadeOut::create(0.6f);
    auto remove = RemoveSelf::create();
    label->runAction(Sequence::create(
        Spawn::create(move, fade, nullptr),
        remove,
        nullptr
    ));
}