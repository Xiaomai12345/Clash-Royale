#ifndef __CARDSELECTINGSCENE_H__
#define __CARDSELECTINGSCENE_H__

#include "cocos2d.h"
#include "CardManager.h"
#include "Button.h"
#include "Card.h"

class CardSelectingScene : public cocos2d::Scene
{
public:
    void toMainMenu(cocos2d::Ref* sender);
    virtual bool init() override;
    static CardSelectingScene* create();
    static cocos2d::Scene* createScene();

    void createBackground();
    void createButton();
    void initSelectedCards();
    void initUnselectedCards();
    void playMusic();

    // 触摸拖拽回调
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    // 刷新函数
    void refreshSelectedCards();
    void refreshUnselectedCards();

private:
    
    //备战区状态保存/加载
    void saveSelectedCardsState(); // 保存备战区状态
    void loadSelectedCardsState();  // 加载备战区状态

    Sprite* background;
    //备战区两行四列，最大数为8
    const int _maxSelectedCount;
    Vec2 _selectedStartPos;      // 备战区（上方）起始位置
    Vec2 _unselectedStartPos;    // 仓库（下方）起始位置
    int _slotSpacing;            // 列间距
    Size _slotSize;              // 槽位尺寸
    const int _maxRowCount;      // 每行最大卡牌数（4，不变）
    int _rowSpacing;             // 行间距
    int _dragOriginalZOrder; // 记录拖拽卡牌的原始层级

    Card* _draggingCard;
    Sprite* _cardGhost;
    Vec2 _dragStartPos;



    // 辅助函数：判断触摸位置是否在已选区域内 + 获取已选区空槽位数量
    bool isInSelectedArea(Vec2 worldPos);
    int getSelectedEmptySlotCount();

    // 构造函数
    CardSelectingScene() : _maxSelectedCount(8), _maxRowCount(4)
    {
        background = nullptr;
        _slotSpacing = 200;    // 列间距
        _rowSpacing = 125;      // 行间距
        _draggingCard = nullptr;
        _cardGhost = nullptr;
        _dragOriginalZOrder = 0; // 初始化
    }
    ~CardSelectingScene() {}

};

#endif // __CARDSELECTINGSCENE_H__
