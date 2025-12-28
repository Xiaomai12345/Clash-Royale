#ifndef __MINIONS_H__
#define __MINIONS_H__

#include "TroopBase.h"
#include "AirMoveComponent.h"
#include "MeleeAttackComponent.h"

class Minions : public TroopBase
{
public:
    Minions();   // 构造函数
    ~Minions();

    virtual bool init() override;
    virtual void update(float dt) override;

    CREATE_FUNC(Minions);
};

#endif // __UNDEAD_TROOP_H__
