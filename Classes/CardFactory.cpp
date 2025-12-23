#include "CardFactory.h"
#include "BattleManager.h"
#include"UnitType.h"
#include"AllCards.h"

USING_NS_CC;

Card* CardFactory::createKnightCard()//1
{
    auto card = Card::create();
    card->retain();

    // 1️。信息
    card->setCardInfo(
        1,          // cardId
        "Knight",   // name
        2       ,    // mana
        "Images/CardsUI/KnightApply.png"
    );
    card->setPlayFunc([](const Vec2& pos, int playerId)
        {
            BattleManager::getInstance()->deployUnit(
                UNIT_KNIGHT,  // 这里直接指定单位类型
                pos,          // 设置位置
                playerId      // 设置玩家阵营
            );
            return true;
        });


    return card;
}

Card* CardFactory::createArcherCard()//2
{
    auto card = Card::create();
    card->retain();
    // 1️。信息
    card->setCardInfo(
        1,          // cardId
        "Archer",   // name
        2       ,    // mana
        "Images/CardsUI/ArcherApply.png"
    );

    card->setPlayFunc([](const Vec2& pos, int playerId)
        {
            BattleManager::getInstance()->deployUnit(
                UNIT_ARCHER,  // 这里直接指定单位类型
                pos,          // 设置位置
                playerId      // 设置玩家阵营
            );
            return true;
        });


    return card;
}

Card* CardFactory::createGiantCard()//3
{
    auto card = Card::create();
    card->retain();
    // 1️。信息
    card->setCardInfo(
        1,          // cardId
        "Giant",   // name
        4       ,     // mana
        "Images/CardsUI/GiantApply.png"
    );
    card->setPlayFunc([](const Vec2& pos, int playerId)
        {
            BattleManager::getInstance()->deployUnit(
                UNIT_GIANT,  // 这里直接指定单位类型
                pos,          // 设置位置
                playerId      // 设置玩家阵营
            );
            return true;
        });


    return card;
}

Card* CardFactory::createValkyrieCard()//4
{
    auto card = Card::create();
    card->retain();
    // 1️。信息
    card->setCardInfo(
        1,          // cardId
        "Valkyrie",   // name
        4         ,   // mana
        "Images/CardsUI/ValkyrieApply.png"
    );
    card->setPlayFunc([](const Vec2& pos, int playerId)
        {
            BattleManager::getInstance()->deployUnit(
                UNIT_VALKYRIE,  
                pos,          // 设置位置
                playerId      // 设置玩家阵营
            );
            return true;
        });


    return card;
}

Card* CardFactory::createDragonBabyCard()//5
{
    auto card = Card::create();
    card->retain();
    // 1️。信息
    card->setCardInfo(
        1,          // cardId
        "DragonBaby",   // name
        4          ,  // mana
        "Images/CardsUI/DragonBabyApply.png"
    );
    card->setPlayFunc([](const Vec2& pos, int playerId)
        {
            BattleManager::getInstance()->deployUnit(
                UNIT_DRAGONBABY,
                pos,          // 设置位置
                playerId      // 设置玩家阵营
            );
            return true;
        });


    return card;
}

Card* CardFactory::createCannonCard()//6 
{
    auto card = Card::create();
    card->retain();
    // 1️。信息
    card->setCardInfo(
        1,          // cardId
        "Cannon",   // name
        4       ,     // mana
        "Images/CardsUI/CannonApply.png"
    );
    card->setPlayFunc([](const Vec2& pos, int playerId)
        {
            BattleManager::getInstance()->deployUnit(
                UNIT_CANNON,
                pos,          // 设置位置
                playerId      // 设置玩家阵营
            );
            return true;
        });


    return card;
}

Card* CardFactory::createSkeletonCard()//7 
{
    auto card = Card::create();
    card->retain();
    // 1️。信息
    card->setCardInfo(
        1,          // cardId
        "Skeleton",   // name
        4         ,   // mana
        "Images/CardsUI/SkeletonApply.png"
    );
    card->setPlayFunc([](const Vec2& pos, int playerId)
        {
            BattleManager::getInstance()->deployUnit(
                UNIT_SKELETON,
                pos,          // 设置位置
                playerId      // 设置玩家阵营
            );
            return true;
        });


    return card;
}

Card* CardFactory::createSkeletonLegionCard()//8 
{
    auto card = Card::create();
    card->retain();
    // 1️。信息
    card->setCardInfo(
        1,          // cardId
        "Skeleton",   // name
        4         ,   // mana
        "Images/CardsUI/SkeletonLegionApply.png"
    );
    card->setPlayFunc([](const Vec2& pos, int playerId)
        {
            BattleManager::getInstance()->deployUnit(
                UNIT_SKELETON_LEGION,
                pos,          // 设置位置
                playerId      // 设置玩家阵营
            );
            return true;
        });


    return card;
}

Card* CardFactory::createSkeletonTombstoneCard()//9 
{
    auto card = Card::create();
    card->retain();
    // 1️。信息
    card->setCardInfo(
        1,          // cardId
        "SkeletonTombstone",   // name
        4          ,  // mana
        "Images/CardsUI/SkeletonTombstoneApply.png"
    );
    card->setPlayFunc([](const Vec2& pos, int playerId)
        {
            BattleManager::getInstance()->deployUnit(
                UNIT_SKELETON_TOMBSTONE,
                pos,          // 设置位置
                playerId      // 设置玩家阵营
            );
            return true;
        });


    return card;
}

Card* CardFactory::createMinionsCard()//10
{
    auto card = Card::create();
    card->retain();
    // 1️。信息
    card->setCardInfo(
        1,          // cardId
        "Minons",   // name
        4       ,     // mana
        "Images/CardsUI/MinionsApply.png"
    );
    card->setPlayFunc([](const Vec2& pos, int playerId)
        {
            BattleManager::getInstance()->deployUnit(
                UNIT_MINIONS,
                pos,          // 设置位置
                playerId      // 设置玩家阵营
            );
            return true;
        });


    return card;
}
Card* CardFactory::createFireBallCard() // 11
{
    auto card = Card::create();
    card->retain();
    // 1️。信息
    card->setCardInfo(
        11,          // cardId
        "Fireball",  // name
        5         ,   // mana
        "Images/CardsUI/FireballApply.png"
    );

    card->setPlayFunc([](const Vec2& pos, int playerId)
        {
            BattleManager::getInstance()->deployUnit(
                UNIT_FIREBALL,
                pos,          // 设置位置
                playerId      // 设置玩家阵营
            );
            return true;
        });

    return card;
}

Card* CardFactory::createSlowDownCard() // 12
{
    auto card = Card::create();
    card->retain();
    // 1️。信息
    card->setCardInfo(
        12,          // cardId
        "SlowDown",  // name
        3          ,  // mana
        "Images/CardsUI/SlowDownApply.png"
    );

    card->setPlayFunc([](const Vec2& pos, int playerId)
        {
            BattleManager::getInstance()->deployUnit(
                UNIT_SLWOWSPELL,
                pos,          // 设置位置
                playerId      // 设置玩家阵营
            );
            return true;
        });

    return card;
}
