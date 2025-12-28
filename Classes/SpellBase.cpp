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

    return true;
}
