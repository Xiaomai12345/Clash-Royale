#ifndef CARD_FACTORY_H
#define CARD_FACTORY_H

#include "Card.h"

class CardFactory//生产卡牌的工厂
{
public:
    static Card* createKnightCard();
    static Card* createArcherCard();
    static Card* createGiantCard();
    static Card* createValkyrieCard();
    static Card* createDragonBabyCard();
    static Card* createCannonCard();
    static Card* createSkeletonCard();
    static Card* createMinionsCard();
    static Card* createSkeletonLegionCard();
    static Card* createSkeletonTombstoneCard();
    static Card* createFireBallCard();
    static Card* createSlowDownCard();


};

#endif
