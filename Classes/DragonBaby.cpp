#include "DragonBaby.h"
#include "AirMoveComponent.h"
#include "AreaAttackComponent.h"
#include "SimpleTroopAIComponent.h"
DragonBaby::DragonBaby()
{
    // 在构造函数中给基类和成员变量赋值
    _moveSpeed = 150.0f;  // 设定飞龙宝宝的基础移动速度
    _maxHp = 500;         // 设定飞龙宝宝的最大血量
    _alertRange = 250.f;   // 设置警戒范围
    _bodyRadius = 20.f;    // 设置碰撞半径
    _camp = ECamp::LEFT;   // 设置阵营为左侧

    _attacktype = AttackType::Both;  // 
    _moveAttack = MoveAttack::Both;  // 设置可攻击地面和空中的目标
    _moveAttacked = MoveAttack::Air; // 设置可被攻击地面和空中的目标
    _moveType = MoveType::Air;    // 设置为飞行移动

    _isDying = false;  // 初始化死亡状态
}

DragonBaby::~DragonBaby()
{
    // 如果有创建的组件，手动清理（这里使用了 new 创建组件，所以要 delete）
    if (_ai) delete _ai;
    if (_move) delete _move;
    if (_attack) delete _attack;
}

bool DragonBaby::init()
{
    if (!TroopBase::init())  // 初始化基类 (TroopBase)
        return false;

    // =========================
    // 1. 创建并绑定组件
    // =========================

    // 创建并绑定AI组件（简单的AI逻辑）
    auto ai = new SimpleTroopAIComponent();
    setAIComponent(ai);

    // 创建并绑定飞行移动组件（用于控制飞龙宝宝的移动）
    auto move = new AirMoveComponent();
    setMoveComponent(move);

    // 创建并绑定区域攻击组件（飞龙宝宝的范围攻击）
    auto attack = new AreaAttackComponent(
        200.0f,   // 攻击范围
        2.0f,     // 攻击间隔
        250,       // 伤害值
        100.0f    // AOE 半径
    );
    setAttackComponent(attack);

    // =========================
    // 2. 添加图片作为士兵表现
    // =========================

    // 创建士兵的Sprite（飞龙宝宝的图片）
    _sprite = Sprite::create("Images/troops/DragonBaby.png");
    if (_sprite)
    {
        CCLOG("DragonBaby: Sprite loaded successfully.");
        addChild(_sprite);  // 将图片添加到当前节点
        _sprite->setScale(0.1f);  // 根据需要调整缩放
    }
    else
    {
        CCLOG("DragonBaby: Sprite load failed!");  // 加载图片失败时输出日志
    }

    // 血条初始化（确保只初始化一次）
    return true;
}
