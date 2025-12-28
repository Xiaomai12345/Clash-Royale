#ifndef __ARCHER_TROOP_H__
#define __ARCHER_TROOP_H__

#include "TroopBase.h"


class ArcherTroop : public TroopBase
{
public:
    ArcherTroop();        
    virtual ~ArcherTroop(); 

    virtual bool init() override;
    virtual void update(float dt) override;

    CREATE_FUNC(ArcherTroop);
};

#endif // __ARCHER_TROOP_H__
