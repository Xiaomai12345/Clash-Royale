#ifndef __CANNON_H__
#define __CANNON_H__

#include "BuildingBase.h"

class Cannon : public BuildingBase
{
public:
    // 使用构造函数进行赋值初始化
    Cannon(float maxHp = 1500, float attackRange = 350.0f, float attackInterval = 1.5f, int attackDamage = 30);
    virtual bool init() override;  // 初始化
    virtual void update(float dt) override; // 重写 update 以处理自动扣血

    CREATE_FUNC(Cannon);  // 使用宏来创建实例

private:
    void setupComponents();  // 初始化组件

private:
    float _attackRange;     // 攻击范围
    float _attackInterval;  // 攻击间隔
    int _attackDamage;      // 单次伤害
    
    // 自动扣血相关
    float _lifeTime = 30.0f;     // 总存活时间 (秒)
    float _decayTimer = 0.0f;    // 计时器
    float _decayInterval = 3.0f; // 扣血间隔 (秒)
};

#endif // __CANNON_H__
