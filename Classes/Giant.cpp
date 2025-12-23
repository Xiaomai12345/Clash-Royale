#include "Giant.h"
#include "SimpleTroopAIComponent.h"
#include "GroundMoveComponent.h"
#include "MeleeAttackComponent.h"

Giant::Giant()
{
    // 在构造函数中给基类和成员变量赋值
    _moveSpeed = 100.0f;   // 设定巨人的基础移动速度（相对较慢）
    _maxHp = 500;         // 设定巨人的最大血量
    _alertRange = 250.f;   // 设置警戒范围
    _bodyRadius = 30.f;    // 设置碰撞半径（比骑士大）
    _camp = ECamp::LEFT;   // 设置阵营为左侧

    _attacktype = AttackType::Building;  // 巨人可以攻击地面和空中的目标
    _moveAttack = MoveAttack::Ground;  // 设置巨人可以攻击地面目标
    _moveAttacked = MoveAttack::Both; // 设置巨人可被地面和空中攻击
    _moveType = MoveType::Ground;    // 设置为地面移动

    _isDying = false;  // 初始化死亡状态
}

Giant::~Giant()
{
    // 如果有创建的组件，手动清理（这里使用了 new 创建组件，所以要 delete）
    if (_ai) delete _ai;
    if (_move) delete _move;
    if (_attack) delete _attack;
}

bool Giant::init()
{
    if (!TroopBase::init())  // 初始化基类 (TroopBase)
        return false;

    // =========================
    // 1. 创建并绑定组件
    // =========================

    // 创建并绑定AI组件（简单的AI逻辑）
    auto ai = new SimpleTroopAIComponent();
    setAIComponent(ai);

    // 创建并绑定地面移动组件（用于控制巨人的移动）
    auto move = new GroundMoveComponent();
    setMoveComponent(move);

    // 创建并绑定近战攻击组件（巨人的近战攻击）
    auto attack = new MeleeAttackComponent(
        50.0f,   // 攻击范围（较大的攻击范围）
        1.5f,    // 攻击间隔（慢一点的攻击间隔）
        500      // 伤害值（较高的伤害）
    );
    setAttackComponent(attack);

    // =========================
    // 2. 添加图片作为士兵表现
    // =========================

    // 创建士兵的Sprite（巨人的图片）
    _sprite = Sprite::create("Images/troops/Giant.png");  // 使用巨人的图片（需替换为正确路径）
    if (_sprite)
    {
        CCLOG("Giant: Sprite loaded successfully.");
        addChild(_sprite);  // 将图片添加到当前节点
        _sprite->setScale(0.1f);  // 根据需要调整缩放
    }
    else
    {
        CCLOG("Giant: Sprite load failed!");  // 加载图片失败时输出日志
    }

    // 血条初始化（确保只初始化一次）
    return true;
}
