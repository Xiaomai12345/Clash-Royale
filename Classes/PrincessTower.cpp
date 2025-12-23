#include "PrincessTower.h"
#include "BuildingAttackComponent.h"
#include "SimpleBuildingAI.h"

USING_NS_CC;

PrincessTower::PrincessTower(float maxHp, float attackRange, float attackInterval, int attackDamage)
    : _attackRange(attackRange)
    , _attackInterval(attackInterval)
    , _attackDamage(attackDamage)
{
    _maxHp = maxHp;  // 设置最大血量
    _hp = _maxHp;    // 当前血量与最大血量一致
    _bodyRadius = 25.0f;  // 设置碰撞半径
    _camp = ECamp::LEFT;  // 设置阵营
    _moveAttack = MoveAttack::Both;
    _moveAttacked = MoveAttack::Both;
    _isDying = false;  // 初始化死亡状态
}

bool PrincessTower::init()
{
    if (!BuildingBase::init())  // 初始化父类
        return false;

    setupComponents();  // 初始化组件

    CCLOG("公主塔初始化完成，位置：(%.0f, %.0f)", getPositionX(), getPositionY());

    return true;
}

void PrincessTower::setupComponents()
{
    // 设置AI组件
    auto ai = new SimpleBuildingAI();
    setAIComponent(ai);

    // 设置攻击组件
    auto attack = new BuildingAttackComponent(
        _attackRange,    // 攻击范围
        _attackInterval, // 攻击间隔
        _attackDamage    // 单次伤害
    );
    setAttackComponent(attack);

    // 设置外观（精灵）
    _sprite = Sprite::create("Images/towers/princess_tower_red.png");
    if (_sprite)
    {
        addChild(_sprite);
        _sprite->setScale(1.0f);
    }
    else
    {
        CCLOG("公主塔图片加载失败");
    }
}
