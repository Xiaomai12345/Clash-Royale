#include "ArcherTroop.h"

// 组件
#include "SimpleTroopAIComponent.h"
#include "GroundMoveComponent.h"
#include "RangedAttackComponent.h"

#include "cocos2d.h"

USING_NS_CC;

ArcherTroop::ArcherTroop()
{

    _moveSpeed = 90.0f;
    _maxHp = 70;
    _alertRange = 300.f;
    _bodyRadius = 12.f;
    _camp = ECamp::LEFT;

    _attacktype = AttackType::Both;
    _moveAttack = MoveAttack::Both;
    _moveAttacked = MoveAttack::Both;
    _moveType = MoveType::Ground;

    _isDying = false;
}

ArcherTroop::~ArcherTroop()
{
    if (_ai)     delete _ai;
    if (_move)   delete _move;
    if (_attack) delete _attack;
}

bool ArcherTroop::init()
{
    if (!TroopBase::init())
        return false;

    // =========================
    // 1. 创建并绑定组件
    // =========================

    // AI
    auto ai = new SimpleTroopAIComponent();
    setAIComponent(ai);

    // 地面移动
    auto move = new GroundMoveComponent();
    setMoveComponent(move);

    // 远程攻击
    auto attack = new RangedAttackComponent(
        100.0f,  // 攻击范围
        1.5f,    // 攻击间隔
        34,      // 伤害
        500.0f   // 弹道速度
    );
    setAttackComponent(attack);

    // =========================
    // 2. 添加图片
    // =========================

    _sprite = Sprite::create("Images/troops/Archer.webp");
    if (_sprite)
    {
        addChild(_sprite);
        _sprite->setScale(0.1f);
    }
    else
    {
        CCLOG("ArcherTroop: sprite load failed");
    }

    return true;
}
