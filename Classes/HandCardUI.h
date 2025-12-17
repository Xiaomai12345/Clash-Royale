#ifndef HAND_CARD_UI_H
#define HAND_CARD_UI_H

#include "cocos2d.h"
#include "Card.h"

class HandCardUI : public cocos2d::Node
{
public:
    CREATE_FUNC(HandCardUI);

    virtual bool init() override;

    // 添加卡牌到手牌
    void addCard(Card* card);

    // 从手牌中移除卡牌
    void removeCard(Card* card);

    // 获取所有手牌
    const cocos2d::Vector<Card*>& getCards() const { return _cards; }

    // 更新手牌布局
    void updateLayout();

private:
    void createBackground();

private:
    cocos2d::Vector<Card*> _cards;
    cocos2d::Sprite* _background;
};

#endif // HAND_CARD_UI_H