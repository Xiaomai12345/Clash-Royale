#ifndef BATTLE_MANAGER_H
#define BATTLE_MANAGER_H

#include "cocos2d.h"
#include "ManaSystem.h"
#include<vector>
class Battlefield;
class UnitBase;

class BattleManager
{
public:
    // 获取单例
    static BattleManager* getInstance();
    static void destroyInstance();

    // 初始化
    void init(Battlefield* battlefield);

    // 更新
    void update(float delta);

    // 单位管理
    void deployUnit(int unitType, const cocos2d::Vec2& position, int playerId);

    // 游戏状态
    void addCrown(int playerId, int crowns = 1);
    int getPlayerCrowns(int playerId) const;

    // 重置
    void reset();

    // 游戏模式
    void setGameMode(const std::string& mode);
    std::string getCurrentGameMode() const { return _currentGameMode; }

    bool getEnemyLeftPrincessAlive()const { return enemyLeftPrincessAlive; }
    bool getEnemyRightPrincessAlive()const { return enemyRightPrincessAlive; }
private:
    BattleManager();
    ~BattleManager();

    BattleManager(const BattleManager&) = delete;
    BattleManager& operator=(const BattleManager&) = delete;

private:
    static BattleManager* _instance;

    Battlefield* _battlefield;
    ManaSystem* _manaSystem;

    // 游戏状态
    bool _gameActive;
    bool _gameEnded;
    float _gameTime;
    int myCrown;
    int enemyCrown;
    bool myLeftPrincessAlive;
    bool enemyLeftPrincessAlive;
    bool myRightPrincessAlive;
    bool enemyRightPrincessAlive;
    bool myKingAlive;
    bool enemyKingAlive;
    std::string _currentGameMode;

    // 玩家数据
    std::unordered_map<int, int> _playerCrowns;
};

#endif // BATTLE_MANAGER_H