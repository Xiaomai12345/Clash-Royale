#ifndef CARD_H
#define CARD_H

#include "cocos2d.h"

class Card : public cocos2d::Node
{
public:
    CREATE_FUNC(Card);

    virtual bool init() override;

    // 设置卡牌信息
    void setCardInfo(int cardId, const std::string& name, float manaCost);

    // 获取属性
    int getCardId() const { return _cardId; }
    const std::string& getName() const { return _name; }
    float getManaCost() const { return _manaCost; }

    // 使用卡牌
    virtual bool use(const cocos2d::Vec2& position, int playerId);

    // UI相关
    void setSelected(bool selected);
    bool isSelected() const { return _isSelected; }

    // ===== 命中检测（用于拖拽）=====
    bool hitTest(const cocos2d::Vec2& worldPos) const;

protected:
    int _cardId;
    std::string _name;
    float _manaCost;
    bool _isSelected;

    // UI元素
    cocos2d::Sprite* _cardSprite;
    cocos2d::Label* _manaLabel;
    cocos2d::Label* _nameLabel;
};

#endif // CARD_H