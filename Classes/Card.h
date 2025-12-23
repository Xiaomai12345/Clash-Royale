#ifndef CARD_H
#define CARD_H

#include "cocos2d.h"
#include <functional>

class Card : public cocos2d::Node
{
public:
    CREATE_FUNC(Card);

    virtual bool init() override;

    /* =========================
     * 卡牌基础信息（只描述）
     * ========================= */
    void setCardInfo(int cardId,
        const std::string& name,
        int manaCost);

    int getCardId() const { return _cardId; }
    const std::string& getName() const { return _name; }
    int getManaCost() const { return _manaCost; }

    /* =========================
     * 释放行为（关键）
     * 由 CardManager / Factory 注入
     * ========================= */
    using PlayFunc = std::function<bool(const cocos2d::Vec2&, int)>;

    void setPlayFunc(const PlayFunc& func);

    // 使用卡牌（UI → 逻辑的唯一入口）
    bool use(const cocos2d::Vec2& worldPos, int playerId);

    /* =========================
     * UI 相关
     * ========================= */
    void setSelected(bool selected);
    bool isSelected() const { return _isSelected; }
    void setCardArt(const std::string& imagePath);

private:
    /* =========================
     * 数据
     * ========================= */
    int _cardId = 0;
    std::string _name;
    int _manaCost = 0;

    PlayFunc _playFunc;   // 🔥 真正的“卡牌效果”

    /* =========================
     * UI 状态
     * ========================= */
    bool _isSelected = false;



    cocos2d::Sprite* _artSprite = nullptr;
    cocos2d::Sprite* _cardSprite = nullptr;
    cocos2d::Label* _manaLabel = nullptr;
    cocos2d::Label* _nameLabel = nullptr;
};

#endif // CARD_H
