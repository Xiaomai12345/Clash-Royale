#include "Cannon.h"
#include "BuildingAttackComponent.h"
#include "SimpleBuildingAI.h"
#include"AnimationComponent.h"
USING_NS_CC;

Cannon::Cannon(float maxHp, float attackRange, float attackInterval, int attackDamage)
    : _attackRange(attackRange)
    , _attackInterval(attackInterval)
    , _attackDamage(attackDamage)
{
    _maxHp = maxHp;
    _hp = _maxHp;
    _bodyRadius = 20.0f;
    _camp = ECamp::LEFT;
    _moveAttack = MoveAttack::Ground; // 只能攻击地面
    _moveAttacked = MoveAttack::Both;
    _isDying = false;
    _shouldRotate = true; // 加农炮需要旋转
    _rooted = true; // 可移动
}

bool Cannon::init()
{
    if (!BuildingBase::init())
        return false;

    // 确保血量正确
    if (_maxHp <= 0) _maxHp = 1500;
    _hp = _maxHp;

    setupComponents();

    CCLOG("Cannon 初始化完成，位置：(%.0f, %.0f) HP: %d", getPositionX(), getPositionY(), _hp);

    return true;
}

void Cannon::setupComponents()
{
    // 设置AI组件
    auto ai = new SimpleBuildingAI();
    setAIComponent(ai);

    // 设置攻击组件
    auto attack = new BuildingAttackComponent(
        _attackRange,    // 攻击范围
        _attackInterval, // 攻击间隔
        _attackDamage,   // 攻击伤害 (使用成员变量)
        500.0f           // 弹道速度
    );
    setAttackComponent(attack);

    // 设置外观
    float baseScale = 0.8f; // 调整缩放比例以匹配新素材

    // 1. 炮管 (旋转部分，赋值给 _sprite 以便 BuildingBase 控制)
    _sprite = Sprite::create("Images/troops/Animations/CannonBarrel.png");
    if (_sprite)
    {
        addChild(_sprite, 1); // 层级 1：上层
        _sprite->setScale(baseScale);
        
        // 炮管保持中心 (0.5, 0.5)，围绕中心旋转
        _sprite->setAnchorPoint(Vec2(0.5f, 0.5f)); 
    }
    else
    {
        CCLOG("CannonTurret 图片加载失败");
        // 回退方案：如果新图加载失败，尝试加载旧图
        _sprite = Sprite::create("Images/Buildings/Cannon.jpg");
        if (_sprite) {
            addChild(_sprite, 1);
            _sprite->setScale(0.07f);
        }
    }

    // 2. 底座 (静止不动)
    auto baseSprite = Sprite::create("Images/troops/Animations/Cannonbase.png");
    if (baseSprite)
    {
        addChild(baseSprite, 0); // 层级 0：底层
        baseSprite->setScale(baseScale);
        
        // 调整底座位置以对齐炮管 (目前设为 0,0 中心对齐，可根据需要调整 x, y)
        baseSprite->setPosition(Vec2(0, -10)); 
    }
    else
    {
        CCLOG("CannonBase 图片加载失败");
    }

    // 设置动画组件 (主要是为了后坐力效果)
    auto anim = new AnimationComponent();
    anim->setTargetSprite(_sprite); // ⚠️ 关键修正：设置动画目标 Sprite
    setAnimationComponent(anim);

    // 攻击状态：模拟后坐力 + 炮口火焰
    // 放大 -> 还原 -> 等待
    if (_sprite)
    {
		CCLOG("展现攻击动画效果");
        // 1. 后坐力效果
        auto recoilAction = Sequence::create(
            ScaleTo::create(0.05f, baseScale * 1.2f), // 快速后坐力
            ScaleTo::create(0.1f, baseScale),         // 复位
            DelayTime::create(MAX(0, _attackInterval - 0.15f)), // 冷却
            nullptr
        );

        // 2. 炮口闪光效果 (变色)
        auto flashAction = Sequence::create(
            TintTo::create(0.05f, 255, 200, 100), // 变亮黄
            TintTo::create(0.1f, 255, 255, 255),  // 恢复原色
            DelayTime::create(MAX(0, _attackInterval - 0.15f)),
            nullptr
        );

        // 3. 组合动作
        auto attackAnim = Spawn::create(recoilAction, flashAction, nullptr);
        
        anim->addAction(State::ATTACKING, RepeatForever::create(attackAnim));
    }
}
