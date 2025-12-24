#include "Minions.h"
#include "AirMoveComponent.h"
#include "MeleeAttackComponent.h"
#include "SimpleTroopAIComponent.h"

USING_NS_CC;

Minions::Minions()
{
    // =========================
    // 基础属性初始化
    // =========================
    _moveSpeed = 180.0f;     // 亡灵移动速度（比飞龙快）
    _maxHp = 90;             // 亡灵血量较低
    _alertRange = 220.f;     // 警戒范围
    _bodyRadius = 12.f;      // 碰撞半径偏小
    _camp = ECamp::LEFT;     // 左侧阵营

    _attacktype = AttackType::Both; // 可攻击建筑和单位
    _moveAttack = MoveAttack::Both; // 可攻击空中和地面
    _moveAttacked = MoveAttack::Air; // 只能被对空攻击
    _moveType = MoveType::Air;      // 飞行单位

    _isDying = false;
}

Minions::~Minions()
{
    // 组件是 new 出来的，必须手动 delete
    if (_ai)     delete _ai;
    if (_move)   delete _move;
    if (_attack) delete _attack;
}

bool Minions::init()
{
    if (!TroopBase::init())
        return false;

    // =========================
    // 1. 组件绑定
    // =========================

    // AI
    auto ai = new SimpleTroopAIComponent();
    setAIComponent(ai);

    // 飞行移动
    auto move = new AirMoveComponent();
    setMoveComponent(move);

    // 近战攻击
    auto attack = new MeleeAttackComponent(
        20.0f,   // 攻击范围（很近）
        0.8f,    // 攻击间隔（快攻）
        45       // 伤害
    );
    setAttackComponent(attack);

    // =========================
    // 2. Sprite
    // =========================

    _sprite = Sprite::create("Images/troops/Minions.webp");
    if (_sprite)
    {
        CCLOG("Minions: Sprite loaded successfully.");
        addChild(_sprite);
        _sprite->setScale(0.08f);
    }
    else
    {
        CCLOG("Minions: Sprite load failed!");
    }

    return true;
}
