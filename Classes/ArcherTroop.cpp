#include "ArcherTroop.h"

// 组件
#include "SimpleTroopAIComponent.h"
#include "GroundMoveComponent.h"
#include "RangedAttackComponent.h"

#include "cocos2d.h"

bool ArcherTroop::init()
{
    if (!TroopBase::init())  // 初始化基类 (TroopBase)
        return false;

    // =========================
    // 1. 创建并绑定组件
    // =========================

    // 创建并绑定AI组件 (简单的AI逻辑)
    auto ai = new SimpleTroopAIComponent();
    setAIComponent(ai);

    // 创建并绑定地面移动组件 (用于控制弓箭手的移动)
    auto move = new GroundMoveComponent();
    setMoveComponent(move);

    // 创建并绑定远程攻击组件 (弓箭手的攻击)
    auto attack = new RangedAttackComponent(
        100.0f,  // 攻击范围
        1.5f,    // 攻击间隔
        34,
        // 伤害值
        500.0f   // 弹道速度 (原为 30，太慢了)
    );
    setAttackComponent(attack);

    // =========================
    // 2. 添加图片作为士兵表现
    // =========================

    // 创建士兵的Sprite (弓箭手的图片)
    _sprite = Sprite::create("Images/troops/Archer.webp");  // 替换为弓箭手的图片
    if (_sprite)
    {
        addChild(_sprite);  // 将图片添加到当前节点
        _sprite->setScale(0.1f);  // 根据需要调整缩放
    }
    else
    {
        CCLOG("ArcherTroop: sprite load failed");  // 加载图片失败时输出日志
    }

    // 仅初始化一次血条，保证士兵初始化时创建血条
    return true;
}
