// SpellBase.cpp
#include "SpellBase.h"

SpellBase::SpellBase()
{
}

SpellBase::~SpellBase()
{
}

bool SpellBase::init()
{
    if (!Node::init())
        return false;

    // Spell 默认不需要 update
    // 如果子类需要动画，可以自己 scheduleUpdate
    return true;
}
