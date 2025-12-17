#include "CardManager.h"

USING_NS_CC;

// 静态成员初始化
CardManager* CardManager::_instance = nullptr;

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

CardManager::CardManager()
    : _drawTimer(0.0f)
    , _drawInterval(2.8f)
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
    _handCards.clear();
    _deck.clear();
    _discardPile.clear();

    // 初始抽4张牌（简化版本）
    for (int i = 0; i < 4; i++)
    {
        if (canDrawCard())
        {
            drawCard();
        }
    }

    CCLOG("CardManager initialized");
}

bool CardManager::canDrawCard() const
{
    return _handSize < _maxHandSize;
}

Card* CardManager::drawCard()
{
    if (!canDrawCard())
    {
        CCLOG("Hand is full, cannot draw more cards");
        return nullptr;
    }

    // 简化版本：创建一个空的卡牌对象
    Card* card = Card::create();
    if (card)
    {
        _handCards.push_back(card);
        _handSize++;
    }

    return card;
}

bool CardManager::useCard(Card* card, const cocos2d::Vec2& position, int playerId)
{
    if (!card)
        return false;

    // 从手牌中移除
    auto it = std::find(_handCards.begin(), _handCards.end(), card);
    if (it != _handCards.end())
    {
        _handCards.erase(it);
        _handSize--;
        return true;
    }

    return false;
}

void CardManager::update(float delta)
{
    _drawTimer += delta;

    if (_drawTimer >= _drawInterval && canDrawCard())
    {
        drawCard();
        _drawTimer = 0.0f;
    }
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