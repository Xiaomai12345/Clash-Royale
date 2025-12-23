#ifndef __MINIONS_H__
#define __MINIONS_H__

#include "TroopBase.h"
#include "AirMoveComponent.h"
#include "MeleeAttackComponent.h"

// UndeadTroop
// -----------
// 亡灵
// 飞行单位，近战攻击
//
class Minions : public TroopBase
{
public:
    Minions();   // 构造函数
    ~Minions();

    virtual bool init() override;

    CREATE_FUNC(Minions);
};

#endif // __UNDEAD_TROOP_H__
