#ifndef __VALKYRIE_TROOP_H__
#define __VALKYRIE_TROOP_H__

#include "TroopBase.h"

// ValkyrieTroop
// -------------
// 女武神
// 地面单位，近战范围攻击（AOE）
// 典型：高血量、中速、群体清场
//
class ValkyrieTroop : public TroopBase
{
public:
    ValkyrieTroop();
    ~ValkyrieTroop();

    virtual bool init() override;
    virtual void update(float dt) override;

    CREATE_FUNC(ValkyrieTroop);
};

#endif // __VALKYRIE_TROOP_H__
