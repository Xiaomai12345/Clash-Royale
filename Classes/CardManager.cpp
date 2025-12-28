#include "CardManager.h"
#include <algorithm>
#include <random>     
USING_NS_CC;

// 静态成员初始化
CardManager* CardManager::_instance = nullptr;
CardManager* CardManager::_enemyInstance = nullptr;

CardManager* CardManager::getInstance()
{
    if (!_instance)
    {
        _instance = new CardManager();
    }
    return _instance;
}

void CardManager::destroyInstance()
{
    if (_instance)
    {
        delete _instance;
        _instance = nullptr;
    }
}

CardManager* CardManager::getEnemyInstance()
{
    if (!_enemyInstance)
    {
        _enemyInstance = new CardManager();
    }
    return _enemyInstance;
}

void CardManager::destroyEnemyInstance()
{
    if (_enemyInstance)
    {
        delete _enemyInstance;
        _enemyInstance = nullptr;
    }
}

CardManager::CardManager()
    : _drawTimer(0.0f)
    , _drawInterval(0.5f)
    , _maxHandSize(8)
    , _handSize(0)
{
}

CardManager::~CardManager()
{
    reset();
}

void CardManager::init()
{
    _drawTimer = 0.0f;
    _handSize = 0;

    // 清空所有牌
    initDeck();
    initHandCards();
    initDiscardPile();
    _nextCard = _discardPile[0];

    CCLOG("CardManager initialized");
}
void CardManager::initDeck()
{
    _deck.clear();
    Card* knight = CardFactory::createKnightCard();
    Card* archer = CardFactory::createArcherCard();
    Card* giant = CardFactory::createGiantCard();
    Card* valkyrie = CardFactory::createValkyrieCard();
    Card* dragonbaby = CardFactory::createDragonBabyCard();
    Card* cannon = CardFactory::createCannonCard();
    Card* skeleton = CardFactory::createSkeletonCard();
    Card* minions = CardFactory::createSkeletonTombstoneCard();

    _deck.push_back(knight);
    _deck.push_back(archer);
    _deck.push_back(giant);
    _deck.push_back(valkyrie);
    _deck.push_back(dragonbaby);
    _deck.push_back(cannon);
    _deck.push_back(skeleton);
    _deck.push_back(minions);

    // 随机打乱牌组
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(_deck.begin(), _deck.end(), g);
}
void CardManager::initHandCards()
{
    _handCards.clear();
    for (int i = 0; i < 4; ++i)
    {
        _handCards.push_back(_deck[i]);
    }
}
void CardManager::initDiscardPile()
{
    _discardPile.clear();
    for (int i = 4; i < 8; i++)
    {
        _discardPile.push_back(_deck[i]);
    }
}




bool CardManager::useCard(Card* card)
{
    if (!card) return false;

    for (int i = 0; i < 4; ++i)
    {
        if (_handCards[i] == card)
        {
            // 1. 当前槽位清空
            _handCards[i] = nullptr;

            card->setVisible(false);
            // 2. 卡回牌库尾（循环）
            _discardPile.push_back(card);

            _drawTimer = 0.0f;
            return true;
        }
    }
    return false;
}


void CardManager::update(float delta)
{
    _drawTimer += delta;

    if (_drawTimer < _drawInterval) return;

    for (int i = 0; i < 4; ++i)
    {
        if (_handCards[i] == nullptr && !_discardPile.empty())
        {

            _handCards[i] = _discardPile[0];
            _discardPile.erase(_discardPile.begin());
            _nextCard = _discardPile[0];
            break; // 一次只补一张
        }
    }
    _drawTimer = 0.0f;
}

void CardManager::reset()
{
    // 清理所有卡牌
    for (auto card : _handCards)
    {
        if (card)
        {
            card->release();
        }
    }

    for (auto card : _deck)
    {
        if (card)
        {
            card->release();
        }
    }

    for (auto card : _discardPile)
    {
        if (card)
        {
            card->release();
        }
    }

    _handCards.clear();
    _deck.clear();
    _discardPile.clear();

    _drawTimer = 0.0f;
    _handSize = 0;
}

Card* CardManager::getCardAtWorldPos(const cocos2d::Vec2& pos)
{
    for (auto it = _handCards.rbegin(); it != _handCards.rend(); ++it)
    {
        Card* card = *it;
        if (!card) continue;                 // 防 nullptr
        if (!card->isVisible()) continue;
        if (!card->getParent()) continue;    // 防已移除

        if (card->hitTest(pos))
            return card;
    }
    return nullptr;
}