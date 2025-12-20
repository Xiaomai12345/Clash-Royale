#ifndef __SIMPLE_TROOP_AI_COMPONENT_H__
#define __SIMPLE_TROOP_AI_COMPONENT_H__

#include "TroopAIComponent.h"

class TroopBase;

class SimpleTroopAIComponent : public TroopAIComponent
{
public:
    SimpleTroopAIComponent();


    virtual void update(TroopBase* owner, float dt) override;

};

#endif
