#ifndef __CANNON_H__
#define __CANNON_H__

#include "BuildingBase.h"

class Cannon : public BuildingBase
{
public:
    // 使用构造函数进行赋值初始化
    Cannon(float maxHp = 1500, float attackRange = 350.0f, float attackInterval = 1.5f, int attackDamage = 30);
    virtual bool init() override;  // 初始化

    CREATE_FUNC(Cannon);  // 使用宏来创建实例

private:
    void setupComponents();  // 初始化组件

private:
    float _attackRange;     // 攻击范围
    float _attackInterval;  // 攻击间隔
    int _attackDamage;      // 单次伤害
};

#endif // __CANNON_H__
