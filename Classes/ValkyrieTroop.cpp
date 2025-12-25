#include "ValkyrieTroop.h"
#include "SimpleTroopAIComponent.h"
#include "GroundMoveComponent.h"
#include "MeleeAreaAttackComponent.h"
#include "cocos2d.h"

USING_NS_CC;

ValkyrieTroop::ValkyrieTroop()
{
    // =========================
    // 基础属性
    // =========================
    _moveSpeed = 90.0f;    // ����ʿ����
    _maxHp = 200;      // Ů����Ѫ��
    _alertRange = 220.f;
    _bodyRadius = 18.f;

    _camp = ECamp::LEFT;

    _attacktype = AttackType::Both;
    _moveAttack = MoveAttack::Ground;
    _moveAttacked = MoveAttack::Both;
    _moveType = MoveType::Ground;

    _isDying = false;
}

ValkyrieTroop::~ValkyrieTroop()
{
    if (_ai)     delete _ai;
    if (_move)   delete _move;
    if (_attack) delete _attack;
}

bool ValkyrieTroop::init()
{
    if (!TroopBase::init())
        return false;

    // =========================
    // 1. 组件绑定
    // =========================

    // AI
    auto ai = new SimpleTroopAIComponent();
    setAIComponent(ai);

    // 地面移动
    auto move = new GroundMoveComponent();
    setMoveComponent(move);

    // 近战范围攻击（核心区别）
    auto attack = new MeleeAreaAttackComponent(
        45.0f,   // ������Χ��AOE �뾶��
        1.2f,    // �������
        30      // �����˺�
    );
    setAttackComponent(attack);

    // =========================
    // 2. 表现（Sprite）
    // =========================

    _sprite = Sprite::create("Images/troops/valkyrie.webp");
    if (_sprite)
    {
        addChild(_sprite);
        _sprite->setScale(0.06f);
    }
    else
    {
        CCLOG("ValkyrieTroop: Sprite load failed!");
    }

    return true;
}
