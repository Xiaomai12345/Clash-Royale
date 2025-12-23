锘�#ifndef CARD_H
#define CARD_H

#include "cocos2d.h"
#include <functional>

class Card : public cocos2d::Node
{
public:
    CREATE_FUNC(Card);

    virtual bool init() override;

    /* =========================
     * 鍗＄墝鍩虹淇℃伅锛堝彧鎻忚堪锛�
     * ========================= */
    void setCardInfo(int cardId,
        const std::string& name,
        int manaCost);

    int getCardId() const { return _cardId; }
    const std::string& getName() const { return _name; }
    int getManaCost() const { return _manaCost; }

    /* =========================
     * 閲婃斁琛屼负锛堝叧閿級
     * 鐢� CardManager / Factory 娉ㄥ叆
     * ========================= */
    using PlayFunc = std::function<bool(const cocos2d::Vec2&, int)>;

    void setPlayFunc(const PlayFunc& func);

    // 浣跨敤鍗＄墝锛圲I 鈫� 閫昏緫鐨勫敮涓�鍏ュ彛锛�
    bool use(const cocos2d::Vec2& worldPos, int playerId);

    /* =========================
     * UI 鐩稿叧
     * ========================= */
    void setSelected(bool selected);
    bool isSelected() const { return _isSelected; }
    void setCardArt(const std::string& imagePath);

private:
    /* =========================
     * 鏁版嵁
     * ========================= */
    int _cardId = 0;
    std::string _name;
    int _manaCost = 0;

    PlayFunc _playFunc;   // 馃敟 鐪熸鐨勨�滃崱鐗屾晥鏋溾��

    /* =========================
     * UI 鐘舵��
     * ========================= */
    bool _isSelected = false;



    cocos2d::Sprite* _artSprite = nullptr;
    cocos2d::Sprite* _cardSprite = nullptr;
    cocos2d::Label* _manaLabel = nullptr;
    cocos2d::Label* _nameLabel = nullptr;
};

#endif // CARD_H
