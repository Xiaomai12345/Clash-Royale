#include "CardSelectingScene.h"
#include "MainMenuScene.h"
#include "GameManager.h"
#include"AudioManager.h"
#include"DataManager.h"
#include "json/document.h"
#include "json/writer.h"
USING_NS_CC;

using namespace rapidjson; 

class Card;
class GameButton;


CardSelectingScene* CardSelectingScene::create()
{
    CardSelectingScene* ret = new CardSelectingScene();
    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        delete ret;
        return nullptr;
    }
}

bool CardSelectingScene::init()
{
    if (!Scene::init()) {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();

    //仓库和备战区位置初始化
    _selectedStartPos = Vec2(100, 1050);
    _unselectedStartPos = Vec2(100, 250);
    _slotSize = Size(125, 165);

    // 添加全局触摸监听器
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(CardSelectingScene::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(CardSelectingScene::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(CardSelectingScene::onTouchEnded, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);


    createBackground();//创建背景

    createButton();//放置按钮

    // 初始化数据管理器和卡牌管理器
    DataManager::getInstance()->loadCardConfig("card_config.json"); // 替换为你的JSON路径
    auto cardMgr = CardManager::getInstance();
    cardMgr->init();
    cardMgr->initWarehouseCards();

    // 初始化手牌显示
    initSelectedCards();

    //初始化备战区卡牌显示
    initUnselectedCards();

    // ===================== 新增：加载备战区状态（覆盖默认初始化） =====================
    this->loadSelectedCardsState();

    // 刷新布局（确保加载后UI正常显示）
    refreshSelectedCards();
    refreshUnselectedCards();

    return true;
}

//切换到主界面
void CardSelectingScene::toMainMenu(Ref* sender)
{
    // ===================== 新增：保存备战区状态 =====================
    this->saveSelectedCardsState();

    Director::getInstance()->replaceScene(MainMenuScene::create());
}
//用于主程序调用
Scene* CardSelectingScene::createScene()
{
    return CardSelectingScene::create();
}

//创建按钮
void CardSelectingScene::createButton()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 screenCenter = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    
    //创建切换到主页面的按钮
    auto MainBtn = GameButton::create(
        "Images/button/Main.png",    // 正常态图片路径
        "Images/button/Main_selected.png",   // 按下态图片路径
        "Images/button/Main_disabled.png",  // 禁用态图片路径
        "",                         // 按钮文字
        "",            // 字体路径
        0,                                   // 文字大小
        [=]() {
            CCLOG("点击了切换卡牌页面按钮！");
            this->toMainMenu(nullptr);
        }
    );
    
    auto CardtBtn = GameButton::create(
        "Images/button/cards.png",   
        "Images/button/cards_selected.png",   
        "Images/button/cards_disabled.png", 
        "",                        
        "",          
        0,                                  
        nullptr
    );
    // 设置按钮位置
    MainBtn->setPosition(Vec2(screenCenter.x, screenCenter.y * 0.138));
    MainBtn->setScale(0.5);

    CardtBtn->setPosition(Vec2(screenCenter.x - 180, screenCenter.y * 0.08));
    CardtBtn->setScale(0.5);
    CardtBtn->setBtnEnabled(false);

    this->addChild(MainBtn);
    this->addChild(CardtBtn);
    
    return;
}


//*创建主界面背景图
void CardSelectingScene::createBackground()
{
    background = Sprite::create("Images/background/CardSelectingScene.png");
    if (!background) {
        CCLOG("选卡界面背景图加载失败！检查路径：Images/background/CardSelectingScene.png");
        return;
    }
    CCLOG("选卡界面背景图加载成功，原始尺寸：%.0f x %.0f", background->getContentSize().width, background->getContentSize().height);
    Size winSize = Director::getInstance()->getVisibleSize(); // 主界面用窗口尺寸
    float scaleX = winSize.width / background->getContentSize().width;
    float scaleY = winSize.height / background->getContentSize().height;
    background->setScale(MIN(scaleX, scaleY)); // 取最大比例

    CCLOG("背景缩放后尺寸：%.0f x %.0f",
        background->getContentSize().width * scaleX,
        background->getContentSize().height * scaleY);

    //用窗口尺寸计算位置，锚点适配主界面
    background->setAnchorPoint(Vec2(0.5f, 0.5f)); // 锚点设中心
    background->setPosition(winSize.width / 2, winSize.height / 2); // 背景居中

    addChild(background, 0); // 层级0，最底层
}


//已选卡牌槽位+卡牌布局
void CardSelectingScene::initSelectedCards()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    auto cardMgr = CardManager::getInstance();
    auto selectedCards = cardMgr->getSelectedCards();

    // 创建已选卡牌槽位（每4个换行）
    for (int i = 0; i < _maxSelectedCount; ++i)
    {
        int colIndex = i % _maxRowCount;    // 列索引
        int rowIndex = i / _maxRowCount;    // 行索引

        //X=起始X+列索引*列间距；Y=起始Y-行索引*(槽位高度+行间距)
        float slotX = _selectedStartPos.x + colIndex * _slotSpacing;
        // Y轴递减：屏幕坐标系向上为正，换行往下排需减少Y值
        float slotY = _selectedStartPos.y - rowIndex * (_slotSize.height + _rowSpacing);

        auto selectedSlot = LayerColor::create(Color4B(86, 59, 41, 255), _slotSize.width, _slotSize.height);
        selectedSlot->setPosition(Vec2(slotX, slotY));
        this->addChild(selectedSlot, 50);
        CCLOG("创建已选卡牌槽位 %d，行列：(%d,%d)，位置：(%.0f, %.0f)", i, colIndex, rowIndex, slotX, slotY);
    }

    // 预添加上场卡牌
    for (auto card : selectedCards)
    {
        if (card && !card->getParent())
        {
            this->addChild(card, 100);
            card->setVisible(false);
        }
    }

    refreshSelectedCards();
}

void CardSelectingScene::initUnselectedCards()
{
    auto cardMgr = CardManager::getInstance();
    auto unselectedCards = cardMgr->getWarehouseCards();

    // 预添加未选卡牌
    for (auto card : unselectedCards)
    {
        if (card && !card->getParent())
        {
            this->addChild(card, 100);
            card->setVisible(false);
        }
    }

    // 创建仓库槽位
    for (int i = 0; i < _maxRowCount; ++i) // i<4，只创建4个槽位
    {
        int colIndex = i % _maxRowCount;
        int rowIndex = 0; // 强制第0行

        float slotX = _unselectedStartPos.x + colIndex * _slotSpacing;
        float slotY = _unselectedStartPos.y - rowIndex * (_slotSize.height + _rowSpacing);

        auto unselectedSlot = LayerColor::create(Color4B(86, 59, 41, 180), _slotSize.width, _slotSize.height);
        unselectedSlot->setPosition(Vec2(slotX, slotY));
        this->addChild(unselectedSlot, 50);
    }

    refreshUnselectedCards();
}
// 3. 刷新已选卡牌
void CardSelectingScene::refreshSelectedCards()
{
    auto cardMgr = CardManager::getInstance();
    auto selectedCards = cardMgr->getSelectedCards();

    for (int i = 0; i < (int)selectedCards.size() && i < _maxSelectedCount; ++i)
    {
        Card* card = selectedCards[i];
        if (!card) continue;

        if (!card->getParent())
        {
            this->addChild(card, 100);
        }

        // 换行坐标计算
        int colIndex = i % _maxRowCount;
        int rowIndex = i / _maxRowCount;
        float cardX = _selectedStartPos.x + colIndex * _slotSpacing;
        float cardY = _selectedStartPos.y - rowIndex * (_slotSize.height + _rowSpacing);

        card->setPosition(Vec2(cardX, cardY));
        card->setScale(1.0f);
        card->setVisible(true);
        card->refreshView();

    }
}

// 刷新仓库卡牌
void CardSelectingScene::refreshUnselectedCards()
{
    auto cardMgr = CardManager::getInstance();
    auto unselectedCards = cardMgr->getWarehouseCards();

    // 只显示前4张卡牌
    for (int i = 0; i < (int)unselectedCards.size() && i < _maxRowCount; ++i)
    {
        Card* card = unselectedCards[i];
        if (!card) continue;

        if (!card->getParent())
        {
            this->addChild(card, 100);
        }

        int colIndex = i % _maxRowCount;
        int rowIndex = 0; // 强制第0行（不换行）

        float cardX = _unselectedStartPos.x + colIndex * _slotSpacing;
        float cardY = _unselectedStartPos.y - rowIndex * (_slotSize.height + _rowSpacing);

        card->setPosition(Vec2(cardX, cardY));
        card->setScale(1.0f);
        card->setVisible(true);
        card->refreshView();
    }
}

// 判断触摸位置是否在已选卡牌区域内
bool CardSelectingScene::isInSelectedArea(Vec2 worldPos)
{
    // 备战区：两行四列
    float slotWidth = _slotSize.width;
    float slotHeight = _slotSize.height;
    float colSpacingTotal = _maxRowCount * _slotSpacing; // 4列的总宽度
    float rowSpacingTotal = (_slotSize.height + _rowSpacing); // 单行高度（槽位高+行间距）
    float twoRowTotalHeight = 2 * rowSpacingTotal; // 两行总高度

    // 判定矩形的左下角y坐标 = 备战区最下方槽位的左下角
    // 判定矩形的height = 两行总高度
    Rect selectedArea = Rect(
        _selectedStartPos.x, // 矩形左下角x
        _selectedStartPos.y - twoRowTotalHeight + slotHeight, // 矩形左下角y
        colSpacingTotal, // 矩形宽度
        twoRowTotalHeight // 矩形高度
    );
    return selectedArea.containsPoint(worldPos);
}

// 获取已选区空槽位数量
int CardSelectingScene::getSelectedEmptySlotCount()
{
    auto cardMgr = CardManager::getInstance();
    auto selectedCards = cardMgr->getSelectedCards();
    return _maxSelectedCount - (int)selectedCards.size();
}

// 1. 触摸开始
bool CardSelectingScene::onTouchBegan(Touch* touch, Event* event)
{
    Vec2 touchPos = touch->getLocation();
    auto cardMgr = CardManager::getInstance();

    // 获取触摸位置的卡牌
    _draggingCard = cardMgr->getCardAtWorldPos(touchPos);
    if (!_draggingCard)
        return false; // 没点中卡牌，不拦截触摸

    
    //记录卡牌拖拽前的原始层级
    _dragOriginalZOrder = _draggingCard->getLocalZOrder();

    // 设置卡牌拖拽状态（放大+提层级）
    _draggingCard->setScale(1.1f);
    _draggingCard->setLocalZOrder(1000); // 拖拽时临时提层级（在最上层）
    _dragStartPos = _draggingCard->getPosition();

    // 创建拖拽虚影
    if (!_cardGhost)
    {
        _cardGhost = Sprite::create();
        // 虚影样式：半透明白色层，大小匹配卡牌
        auto colorLayer = LayerColor::create(Color4B(255, 255, 255, 120), _slotSize.width, _slotSize.height);
        _cardGhost->addChild(colorLayer);
        this->addChild(_cardGhost, 1500); // 虚影层级高于卡牌
    }
    _cardGhost->setPosition(touchPos);
    _cardGhost->setVisible(true);

    return true; // 拦截触摸事件
}

//2.触摸移动：虚影跟随触摸位置
void CardSelectingScene::onTouchMoved(Touch* touch, Event* event)
{
    if (!_draggingCard || !_cardGhost)
        return;

    Vec2 touchPos = touch->getLocation();
    // 虚影跟随触摸位置
    _cardGhost->setPosition(touchPos);

}

// 3. 触摸结束
void CardSelectingScene::onTouchEnded(Touch* touch, Event* event)
{
    if (!_draggingCard || !_cardGhost)
        return;

    Vec2 touchPos = touch->getLocation();
    auto cardMgr = CardManager::getInstance();
    auto selectedCards = cardMgr->getSelectedCards();
    auto unselectedCards = cardMgr->getWarehouseCards();

    // 是否拖拽到已选区域，且已选区有空位
    bool isInSelected = isInSelectedArea(touchPos);
    bool hasEmptySlot = getSelectedEmptySlotCount() > 0;

    // 判断拖拽卡牌类型
    // 先判断：拖拽的是未选卡牌
    bool isUnselectedCard = false;
    for (auto card : unselectedCards)
    {
        if (card == _draggingCard)
        {
            isUnselectedCard = true;
            break;
        }
    }

    // 情况1：未选卡牌 → 拖入已选区（且有空位）
    if (isUnselectedCard && isInSelected && hasEmptySlot)
    {
        // 从仓库区移除，加入已选区
        cardMgr->removeCardFromWarehouse(_draggingCard);
        cardMgr->addCardToSelected(_draggingCard);
        CCLOG("拖拽：未选卡牌 → 已选区");
    }
    // 情况2：已选卡牌 → 拖出到仓库区（不在已选区域内）
    else if (!isUnselectedCard && !isInSelected)
    {
        // 从已选区移除，加入仓库区
        cardMgr->removeCardFromSelected(_draggingCard);
        cardMgr->addCardToWarehouse(_draggingCard);
        CCLOG("拖拽：已选卡牌 → 未选区");
    }
    // 情况3：拖拽无效→ 卡牌复位
    else
    {
        _draggingCard->setPosition(_dragStartPos); // 回到起始位置
        CCLOG("拖拽无效，卡牌复位");
    }

    // 重置拖拽状态
    _draggingCard->setScale(1.0f); // 恢复原始缩放
    _draggingCard->setLocalZOrder(_dragOriginalZOrder); // 恢复拖拽前的层级
    _draggingCard = nullptr; // 清空拖拽卡牌
    _cardGhost->setVisible(false); // 隐藏虚影

    // 刷新卡牌布局
    refreshSelectedCards();
    refreshUnselectedCards();

    this->saveSelectedCardsState();
}



// 保存备战区状态（序列化卡牌ID列表）
void CardSelectingScene::saveSelectedCardsState()
{
    auto cardMgr = CardManager::getInstance();
    auto selectedCards = cardMgr->getSelectedCards();

    // 1. 构建JSON数组，存储备战区卡牌ID
    Document doc;
    doc.SetArray();
    Document::AllocatorType& allocator = doc.GetAllocator();

    for (auto card : selectedCards)
    {
        if (card) // 避免空指针
        {
            doc.PushBack(card->getCardId(), allocator); // 复用getCardId()接口
        }
    }

    // 2. JSON数组转为字符串，存入UserDefault
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    doc.Accept(writer);
    std::string jsonStr = buffer.GetString();

    // 存储到本地，键名自定义
    UserDefault::getInstance()->setStringForKey("SelectedCards_State_Key", jsonStr);
    UserDefault::getInstance()->flush(); // 强制写入磁盘，确保保存成功

    CCLOG("备战区状态保存成功！JSON：%s", jsonStr.c_str());
}



// 加载备战区状态（反序列化，恢复卡牌）
void CardSelectingScene::loadSelectedCardsState()
{
    auto cardMgr = CardManager::getInstance();
    // 只在无保存状态时，才初始化默认仓库
    std::string jsonStr = UserDefault::getInstance()->getStringForKey("SelectedCards_State_Key", "");
    if (jsonStr.empty())
    {
        CCLOG("无保存的备战区状态，使用默认仓库卡牌");
        // 仅无保存状态时，初始化默认仓库
        if (cardMgr->getWarehouseCards().empty())
        {
            cardMgr->initWarehouseCards();
        }
        return;
    }

    // 解析JSON
    Document doc;
    if (doc.Parse(jsonStr.c_str()).HasParseError() || !doc.IsArray())
    {
        CCLOG("备战区状态解析失败，使用现有仓库卡牌");
        return;
    }

    //先判断仓库是否为空，不为空则不重新初始化
    if (cardMgr->getWarehouseCards().empty())
    {
        cardMgr->initWarehouseCards();
    }
    auto warehouseCards = cardMgr->getWarehouseCards();
    std::vector<Card*> needAddToSelected;

    // 遍历JSON ID，匹配仓库卡牌
    for (SizeType i = 0; i < doc.Size(); ++i)
    {
        if (doc[i].IsInt())
        {
            int targetCardId = doc[i].GetInt();
            for (auto card : warehouseCards)
            {
                if (card && card->getCardId() == targetCardId)
                {
                    // 先判断是否已在备战区，避免重复添加
                    bool isInSelected = false;
                    for (auto sCard : cardMgr->getSelectedCards())
                    {
                        if (sCard == card)
                        {
                            isInSelected = true;
                            break;
                        }
                    }
                    if (!isInSelected)
                    {
                        needAddToSelected.push_back(card);
                    }
                    break;
                }
            }
        }
    }

    // 加入备战区（
    for (auto card : needAddToSelected)
    {
        cardMgr->addCardToSelected(card);
        // 从仓库移除
        cardMgr->removeCardFromWarehouse(card);
    }

    CCLOG("备战区状态加载成功！恢复 %zu 张卡牌", needAddToSelected.size());
}