#ifndef CARD_MANAGER_H
#define CARD_MANAGER_H

#include "cocos2d.h"
#include "Card.h"
#include <vector>
#include <unordered_map>

class Card;

class CardManager
{
public:
    // 获取单例
    static CardManager* getInstance();
    static void destroyInstance();

    // 初始化
    void init();

    // 手牌管理
    std::vector<Card*> getHandCards() const { return _handCards; }
    bool canDrawCard() const;
    Card* drawCard();

    // 卡牌使用
    bool useCard(Card* card, const cocos2d::Vec2& position, int playerId);

    // 更新
    void update(float delta);

    // 重置
    void reset();

private:
    CardManager();
    ~CardManager();

    CardManager(const CardManager&) = delete;
    CardManager& operator=(const CardManager&) = delete;

private:
    static CardManager* _instance;

    // 手牌
    std::vector<Card*> _handCards;
    std::vector<Card*> _deck;
    std::vector<Card*> _discardPile;

    // 卡牌循环
    float _drawTimer;
    float _drawInterval;
    int _maxHandSize;
    int _handSize;
};

#endif // CARD_MANAGER_H