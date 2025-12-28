#include "CardManager.h"
#include <algorithm>
#include <random>     
#include "json/document.h"
#include "json/writer.h"
USING_NS_CC;
using namespace rapidjson;

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

    //初始化仓库
    initWarehouseCards();

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
    Card* FireBall = CardFactory::createFireBallCard();
    Card* SlowDown = CardFactory::createSlowDownCard();

    _deck.push_back(knight);
    _deck.push_back(archer);
    _deck.push_back(giant);
    _deck.push_back(valkyrie);
    _deck.push_back(dragonbaby);
    _deck.push_back(cannon);
    _deck.push_back(skeleton);
    _deck.push_back(minions);
    _deck.push_back(FireBall);
    _deck.push_back(SlowDown);

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
    // 清理仓库和备战区
    for (auto card : _warehouseCards)
    {
        if (card) card->release();
    }
    for (auto card : _selectedCards)
    {
        if (card) card->release();
    }

    _handCards.clear();
    _deck.clear();
    _discardPile.clear();

    _drawTimer = 0.0f;
    _handSize = 0;
}

Card* CardManager::getCardAtWorldPos(const cocos2d::Vec2& pos)
{
    //1. 先遍历备战区（_selectedCards）
        for (auto it = _selectedCards.rbegin(); it != _selectedCards.rend(); ++it)
        {
            Card* card = *it;
            if (!card) continue;
            if (!card->isVisible()) continue;
            if (!card->getParent()) continue;

            Vec2 localPos = card->convertToNodeSpace(pos);
            Rect cardRect = Rect(0, 0, card->getContentSize().width, card->getContentSize().height);
            if (cardRect.containsPoint(localPos))
                return card;
        }

    // 2. 再遍历仓库（_warehouseCards）
    for (auto it = _warehouseCards.rbegin(); it != _warehouseCards.rend(); ++it)
    {
        Card* card = *it;
        if (!card) continue;
        if (!card->isVisible()) continue;
        if (!card->getParent()) continue;

        Vec2 localPos = card->convertToNodeSpace(pos);
        Rect cardRect = Rect(0, 0, card->getContentSize().width, card->getContentSize().height);
        if (cardRect.containsPoint(localPos))
            return card;
    }

    //最后遍历手牌
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



//========== 仓库/已选卡牌  ==========


void CardManager::initWarehouseCards()
{
    _warehouseCards.clear();
    _selectedCards.clear(); // 同时清空备战区

    std::vector<Card*> allDefaultCards;
    Card* knight = CardFactory::createKnightCard();
    Card* archer = CardFactory::createArcherCard();
    Card* giant = CardFactory::createGiantCard();
    Card* valkyrie = CardFactory::createValkyrieCard();
    Card* dragonbaby = CardFactory::createDragonBabyCard();
    Card* cannon = CardFactory::createCannonCard();
    Card* skeleton = CardFactory::createSkeletonCard();
    Card* minions = CardFactory::createSkeletonTombstoneCard();
    Card* FireBall = CardFactory::createFireBallCard();
    Card* SlowDown = CardFactory::createSlowDownCard();

    allDefaultCards.push_back(knight);
    allDefaultCards.push_back(archer);
    allDefaultCards.push_back(giant);
    allDefaultCards.push_back(valkyrie);
    allDefaultCards.push_back(dragonbaby);
    allDefaultCards.push_back(cannon);
    allDefaultCards.push_back(skeleton);
    allDefaultCards.push_back(minions);
    allDefaultCards.push_back(FireBall);
    allDefaultCards.push_back(SlowDown);
    for (auto card : allDefaultCards)
    {
        if (card)
        {
            _warehouseCards.push_back(card);
            card->retain(); // 防止被释放
        }
    }

    CCLOG("仓库卡牌初始化完成！共 %zu 张卡牌", _warehouseCards.size());
}

void CardManager::removeCardFromWarehouse(Card* card)
{
    if (!card) return;
    for (auto it = _warehouseCards.begin(); it != _warehouseCards.end(); ++it)
    {
        if (*it == card)
        {
            _warehouseCards.erase(it);
            break;
        }
    }
}

void CardManager::addCardToSelected(Card* card)
{
    if (!card) return;
    for (auto c : _selectedCards)
    {
        if (c == card) return;
    }
    _selectedCards.push_back(card);
    CCLOG("卡牌加入备战区！当前备战区数量：%zu", _selectedCards.size());
}

void CardManager::removeCardFromSelected(Card* card)
{
    if (!card)
    {
        CCLOG("CardManager：无效卡牌，无法从已选区移除！");
        return;
    }

    for (auto it = _selectedCards.begin(); it != _selectedCards.end(); ++it)
    {
        if (*it == card)
        {
            _selectedCards.erase(it);
            CCLOG("CardManager：卡牌成功从已选区移除！当前已选数量：%zu", _selectedCards.size());
            break;
        }
    }
}

void CardManager::addCardToWarehouse(Card* card)
{
    if (!card)
    {
        CCLOG("CardManager：无效卡牌，无法加入仓库！");
        return;
    }

    for (auto c : _warehouseCards)
    {
        if (c == card)
        {
            CCLOG("CardManager：卡牌已在仓库，无需重复添加！");
            return;
        }
    }

    _warehouseCards.push_back(card);
    CCLOG("CardManager：卡牌成功加入仓库！");
}

// 从备战区状态初始化战斗牌库
void CardManager::initBattleDeckFromSelectedCards()
{
    this->reset();
    _drawTimer = 0.0f;
    _handSize = 0;

    // 读取最新的备战区状态
    std::string jsonStr = UserDefault::getInstance()->getStringForKey("SelectedCards_State_Key", "");
    if (jsonStr.empty())
    {
        CCLOG("警告：无保存的备战区状态，使用默认战斗牌库");
        this->initDeck();
        this->initHandCards();
        this->initDiscardPile();
        _nextCard = !_discardPile.empty() ? _discardPile[0] : nullptr;
        return;
    }

    Document doc;
    if (doc.Parse(jsonStr.c_str()).HasParseError() || !doc.IsArray())
    {
        CCLOG("警告：备战区状态解析失败，使用默认战斗牌库");
        this->initDeck();
        this->initHandCards();
        this->initDiscardPile();
        _nextCard = !_discardPile.empty() ? _discardPile[0] : nullptr;
        return;
    }

    std::vector<Card*> battleCardPool;
    for (SizeType i = 0; i < doc.Size(); ++i)
    {
        if (doc[i].IsInt())
        {
            int cardId = doc[i].GetInt();
            Card* newCard = nullptr;

            // 严格匹配CardFactory cardId
            switch (cardId)
            {
            case 1:  newCard = CardFactory::createKnightCard(); break;
            case 2:  newCard = CardFactory::createArcherCard(); break;
            case 3:  newCard = CardFactory::createGiantCard(); break;
            case 4:  newCard = CardFactory::createValkyrieCard(); break;
            case 5:  newCard = CardFactory::createDragonBabyCard(); break;
            case 6:  newCard = CardFactory::createCannonCard(); break;
            case 7:  newCard = CardFactory::createSkeletonCard(); break;
            case 8:  newCard = CardFactory::createMinionsCard(); break;
            case 9:  newCard = CardFactory::createSkeletonLegionCard(); break;
            case 10: newCard = CardFactory::createSkeletonTombstoneCard(); break;
            case 11: newCard = CardFactory::createFireBallCard(); break;
            case 12: newCard = CardFactory::createSlowDownCard(); break;
            default: CCLOG("未知卡牌ID：%d", cardId); break;
            }

            if (newCard)
            {
                battleCardPool.push_back(newCard);
                newCard->retain();
            }
        }
    }

    if (battleCardPool.empty())
    {
        CCLOG("警告：备战区卡牌池为空，使用默认战斗牌库");
        this->initDeck();
    }
    else
    {
        _deck = battleCardPool;
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(_deck.begin(), _deck.end(), g);
        CCLOG("战斗牌库洗牌完成！基于备战区 %zu 张卡牌打乱顺序", battleCardPool.size());
    }

    this->initHandCards();
    this->initDiscardPile();
    _nextCard = !_discardPile.empty() ? _discardPile[0] : nullptr;

    CCLOG("战斗牌库初始化成功！使用最新备战区卡牌，共 %zu 张", battleCardPool.size());
}