#ifndef __KINGDOM_TOWER_H__
#define __KINGDOM_TOWER_H__

#include "BuildingBase.h"

class KingdomTower : public BuildingBase
{
public:
    // 使用构造函数进行赋值初始化
    KingdomTower(float maxHp = 2000, float attackRange = 400.0f, float attackInterval = 2.f, int attackDamage = 20);
    virtual bool init() override;  // 初始化
    virtual void die() override;   // 重写死亡逻辑

    CREATE_FUNC(KingdomTower);  // 使用宏来创建实例

private:
    void setupComponents();  // 初始化组件

private:
    float _attackRange;     // 攻击范围
    float _attackInterval;  // 攻击间隔
    int _attackDamage;      // 单次伤害
};

#endif // __KINGDOM_TOWER_H__
