#ifndef BATTLE_MANAGER_H
#define BATTLE_MANAGER_H

#include "cocos2d.h"
#include "ManaSystem.h"
#include<vector>
#include <functional>
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
    void addCrown();
    int getPlayerCrowns(int playerId) const;

    // 添加回调类型
    using GameEndCallback = std::function<void(int winnerPlayerId)>;

    // 添加设置回调的方法
    void setGameEndCallback(GameEndCallback callback) { _gameEndCallback = callback; }

    // 重置
    void reset();

    // 游戏模式
    void setGameMode(const std::string& mode) { _currentGameMode = mode; }
    std::string getCurrentGameMode() const { return _currentGameMode; }

    // 获取塔的存活状态
    bool getEnemyLeftPrincessAlive() const { return enemyLeftPrincessAlive; }
    bool getEnemyRightPrincessAlive() const { return enemyRightPrincessAlive; }


	// 设置塔的存活状态
    void setEnemyLeftPrincessAlive(bool alive) { enemyLeftPrincessAlive = alive; CCLOG("敌方左塔被毁int%d ", enemyLeftPrincessAlive); };
	void setEnemyRightPrincessAlive(bool alive) { enemyRightPrincessAlive = alive; CCLOG("敌人右塔被毁int%d ", enemyRightPrincessAlive);};
    void setEnemyKingAlive(bool alive) { enemyKingAlive = alive; CCLOG("敌方国王被毁"); };
	void setMyLeftPrincessAlive(bool alive) { myLeftPrincessAlive = alive; CCLOG("我方左塔被毁int%d ", myLeftPrincessAlive);};
	void setMyRightPrincessAlive(bool alive) { myRightPrincessAlive = alive; CCLOG("我方右塔被毁int%d ", myRightPrincessAlive);};
    void setMyKingAlive(bool alive) { myKingAlive = alive; CCLOG("我方国王被毁"); };
    

private:
    BattleManager();
    ~BattleManager();
    void checkGameEnded();
    BattleManager(const BattleManager&) = delete;
    BattleManager& operator=(const BattleManager&) = delete;

private:
    static BattleManager* _instance;

    Battlefield* _battlefield;
    ManaSystem* _manaSystem;
    ManaSystem* _enemyManaSystem;
    // 游戏状态
    bool _gameActive;
    bool _gameEnded;
    float _gameTime;
    std::vector<bool>myTower;
    std::vector<bool>enemyTower;
    bool myLeftPrincessAlive;
    bool enemyLeftPrincessAlive;
    bool myRightPrincessAlive;
    bool enemyRightPrincessAlive;
    bool myKingAlive;
    bool enemyKingAlive;
    std::string _currentGameMode;
    // 添加回调成员
    GameEndCallback _gameEndCallback;
    // 玩家数据
    std::unordered_map<int, int> _playerCrowns;
};

#endif // BATTLE_MANAGER_H