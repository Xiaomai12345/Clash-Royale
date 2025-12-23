#include "SkeletonTroop.h"
#include "SimpleTroopAIComponent.h"
#include "GroundMoveComponent.h"
#include "MeleeAttackComponent.h"

SkeletonTroop::SkeletonTroop()
{
    // 在构造函数中给基类和成员变量赋值
    _moveSpeed = 90.0f;  // 设定骷髅的基础移动速度（较慢）
    _maxHp = 80;         // 设定骷髅的最大血量
    _alertRange = 150.f;  // 设置警戒范围
    _bodyRadius = 5.f;   // 设置碰撞半径
    _camp = ECamp::LEFT;  // 设置阵营为左侧

    _attacktype = AttackType::Both;  // 骷髅可以攻击地面和空中目标
    _moveAttack = MoveAttack::Ground; // 设置为地面攻击
    _moveAttacked = MoveAttack::Both; // 设置可被攻击地面和空中的目标
    _moveType = MoveType::Ground;    // 设置为地面移动

    _isDying = false;  // 初始化死亡状态
}

SkeletonTroop::~SkeletonTroop()
{
    // 如果有创建的组件，手动清理（这里使用了 new 创建组件，所以要 delete）
    if (_ai) delete _ai;
    if (_move) delete _move;
    if (_attack) delete _attack;
}

bool SkeletonTroop::init()
{
    if (!TroopBase::init())  // 初始化基类 (TroopBase)
        return false;

    setupComponents();  // 初始化组件

    CCLOG("SkeletonTroop 初始化完成，位置：(%.0f, %.0f)", getPositionX(), getPositionY());

    return true;
}

void SkeletonTroop::setupComponents()
{
    // 创建并绑定AI组件（简单的AI逻辑）
    auto ai = new SimpleTroopAIComponent();
    setAIComponent(ai);

    // 创建并绑定地面移动组件（用于控制骷髅的移动）
    auto move = new GroundMoveComponent();
    setMoveComponent(move);

    // 创建并绑定近战攻击组件（骷髅的近战攻击）
    auto attack = new MeleeAttackComponent(
        20.0f,   // 攻击范围
        1.0f,     // 攻击间隔
        40        // 伤害值
    );
    setAttackComponent(attack);

    // 创建骷髅的外观（精灵）
    _sprite = Sprite::create("Images/troops/Skeleton.png");  // 替换为实际路径
    if (_sprite)
    {
        addChild(_sprite);
        _sprite->setScale(0.05f);  // 根据需要调整缩放
    }
    else
    {
        CCLOG("SkeletonTroop: Sprite load failed!");  // 加载图片失败时输出日志
    }
}
