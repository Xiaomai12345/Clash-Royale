#ifndef __VALKYRIE_TROOP_H__
#define __VALKYRIE_TROOP_H__

#include "TroopBase.h"

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
