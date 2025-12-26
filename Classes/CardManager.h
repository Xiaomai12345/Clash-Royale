#ifndef CARD_MANAGER_H
#define CARD_MANAGER_H

#include "cocos2d.h"
#include "Card.h"
#include <vector>
#include <unordered_map>
#include"DataManager.h"
#include"CardFactory.h"
class Card;

class CardManager
{
public:
    // 获取单例
    static CardManager* getInstance();
    static void destroyInstance();
    static CardManager* getEnemyInstance();
    static void destroyEnemyInstance();
    // 初始化
    void init();
    void initDeck();
    void initHandCards();
    void initDiscardPile();

    // 手牌管理
    std::vector<Card*> getHandCards() const { return _handCards; }
    // 卡牌使用
    bool useCard(Card* card);

    // 更新
    void update(float delta);

    // 重置
    void reset();

    Card* getCardAtWorldPos(const cocos2d::Vec2& pos);

    Card* getNextCard()const { return _nextCard; }
private:
    CardManager();
    ~CardManager();

    CardManager(const CardManager&) = delete;
    CardManager& operator=(const CardManager&) = delete;

private:
    static CardManager* _instance;
    static CardManager* _enemyInstance;
    // 手牌
    std::vector<Card*> _handCards;
    std::vector<Card*> _deck;
    std::vector<Card*> _discardPile;

    // 卡牌循环
    float _drawTimer;
    float _drawInterval;
    int _maxHandSize;
    int _handSize;
    Card* _nextCard;
};

#endif // CARD_MANAGER_H