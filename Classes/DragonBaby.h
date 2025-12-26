#ifndef __DRAGON_BABY_H__
#define __DRAGON_BABY_H__

#include "TroopBase.h"
#include "AirMoveComponent.h"
#include "AreaAttackComponent.h"

// DragonBaby
// ----------
// 飞龙宝宝
// 会执行范围攻击，使用飞行组件
//
class DragonBaby : public TroopBase
{
public:
    DragonBaby();  // 声明构造函数
    ~DragonBaby();
    virtual bool init() override;
    virtual void update(float dt) override;

    CREATE_FUNC(DragonBaby);
};

#endif // __DRAGON_BABY_H__
