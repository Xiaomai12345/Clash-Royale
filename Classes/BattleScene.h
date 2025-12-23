#ifndef BATTLE_SCENE_H
#define BATTLE_SCENE_H

#include "cocos2d.h"

class Battlefield;
class Card;
class ManaBar;

class BattleScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init() override;
    virtual void update(float delta) override;
    /*测试datamanager时再打开
    void testDataManager();
    */
    CREATE_FUNC(BattleScene);

private:
    void setupUI();
    void setupInput();
    void startGame();
    void endGame(bool isPlayer1Win);

    // 输入处理 - 修改函数签名
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

    void initHandCards();      // 初始化手牌（只调用一次）
    void refreshHandLayout();  // 重新排布手牌（可多次调用）

    void showDeployPosition();
private:
    Battlefield* _battlefield;
    ManaBar* _manaBar;
    cocos2d::DrawNode* _deployDrawNode;

    // 游戏状态
    bool _gameStarted;
    bool _gameEnded;
    float _gameTime;
    float _totalGameTime;
    // 玩家信息
    int _playerCrowns[2];
    bool _isPlayer1;

    // 选中的卡牌
    Card* _selectedCard;
    cocos2d::Sprite* _cardGhost;
};

#endif // BATTLE_SCENE_H