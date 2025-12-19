#include "KnightTroop.h"
#include "SimpleTroopAIComponent.h"
#include "GroundMoveComponent.h"
#include "MeleeAttackComponent.h"

bool KnightTroop::init()
{
    if (!TroopBase::init())  // 初始化基类 (TroopBase)
        return false;

    // =========================
    // 1. 创建并绑定组件
    // =========================

    // 创建并绑定AI组件（简单的AI逻辑）
    auto ai = new SimpleTroopAIComponent();
    setAIComponent(ai);

    // 创建并绑定地面移动组件（用于控制骑士的移动）
    auto move = new GroundMoveComponent();
    setMoveComponent(move);

    // 创建并绑定近战攻击组件（骑士的近战攻击）
    auto attack = new MeleeAttackComponent(
        50.0f,   // 攻击范围
        1.0f,     // 攻击间隔
        50      // 伤害值
    );
    setAttackComponent(attack);

    // =========================
    // 2. 添加图片作为士兵表现
    // =========================

    // 创建士兵的Sprite（骑士的图片）
    _sprite = Sprite::create("Images/troops/knight.webp");
    if (_sprite)
    {
		CCLOG("KnightTroop: Sprite loaded successfully.");
        addChild(_sprite);  // 将图片添加到当前节点
        _sprite->setScale(0.1f);  // 根据需要调整缩放
    }
    else
    {
        CCLOG("KnightTroop: Sprite load failed!");  // 加载图片失败时输出日志
    }

    // 血条初始化（确保只初始化一次）
    return true;
}
