#ifndef __PRINCESS_TOWER_H__
#define __PRINCESS_TOWER_H__

#include "BuildingBase.h"
// 公主塔 - 简化版本，只包含基本功能用于测试
class PrincessTower : public BuildingBase
{
public:
    /**
     * 重写父类初始化
     */
    virtual bool init() override;

    /**
     * 重写父类更新方法
     */
    virtual void update(float dt) override;

    /**
     * 重写 IAttackable 接口 - 受到伤害
     */
    virtual void takeDamage(int damage) override;

    CREATE_FUNC(PrincessTower);

private:
    // 初始化攻击组件
    void setupAttack();

    // 初始化外观
    void setupAppearance();

    // 显示伤害数字
    void showDamageNumber(int damage);

private:
    float _attackRange = 300.0f;    // 攻击范围
    float _attackInterval = 1.f;   // 攻击间隔
    int _attackDamage = 10;         // 单次伤害
};

#endif // __PRINCESS_TOWER_H__