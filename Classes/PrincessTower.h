#ifndef __PRINCESS_TOWER_H__
#define __PRINCESS_TOWER_H__

#include "BuildingBase.h"

class PrincessTower : public BuildingBase
{
public:
    // 使用构造函数进行赋值初始化
    PrincessTower(float maxHp = 1200, float attackRange = 300.0f, float attackInterval = 1.f, int attackDamage = 10);
    virtual bool init() override;  // 初始化
    virtual void die() override; // 重写死亡处理函数
    virtual void setCamp(ECamp camp) override; // 重写 setCamp 以刷新外观

    CREATE_FUNC(PrincessTower);

private:
    void setupComponents();  // 初始化组件
    void updateAssets();     // 刷新外观资源 (塔身 + 公主)

private:
    float _attackRange;     // 攻击范围
    float _attackInterval;  // 攻击间隔
    int _attackDamage;      // 单次伤害
    
    cocos2d::Sprite* _princessSprite = nullptr; // 持有公主精灵的引用
};

#endif // __PRINCESS_TOWER_H__