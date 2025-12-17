#include "BattleManager.h"
#include "Battlefield.h"

USING_NS_CC;

// 静态成员初始化
BattleManager* BattleManager::_instance = nullptr;

BattleManager* BattleManager::getInstance()
{
    if (!_instance)
    {
        _instance = new BattleManager();
    }
    return _instance;
}

void BattleManager::destroyInstance()
{
    if (_instance)
    {
        delete _instance;
        _instance = nullptr;
    }
}

BattleManager::BattleManager()
    : _battlefield(nullptr)
    , _gameActive(false)
    , _gameEnded(false)
    , _gameTime(0.0f)
    , _currentGameMode("normal")
{
    _manaSystem = ManaSystem::getInstance();

    // 初始化玩家数据
    _playerCrowns[1] = 0;
    _playerCrowns[2] = 0;
}

BattleManager::~BattleManager()
{
}

void BattleManager::init(Battlefield* battlefield)
{
    _battlefield = battlefield;
    _gameActive = true;
    _gameEnded = false;
    _gameTime = 0.0f;

    CCLOG("BattleManager initialized");
}

void BattleManager::update(float delta)
{
    if (!_gameActive || _gameEnded)
        return;

    _gameTime += delta;

    // 检查游戏模式切换
    if (_gameTime >= 60.0f && _currentGameMode == "normal")
    {
        setGameMode("double_elixir");
    }
    else if (_gameTime >= 180.0f && _currentGameMode == "double_elixir")
    {
        setGameMode("sudden_death");
    }
}

void BattleManager::deployUnit(int unitType, const cocos2d::Vec2& position, int playerId)
{
    // 简化版本，实际应该创建具体的单位
    CCLOG("Deploying unit type %d at (%.1f, %.1f) for player %d",
        unitType, position.x, position.y, playerId);
}

void BattleManager::addCrown(int playerId, int crowns)
{
    auto it = _playerCrowns.find(playerId);
    if (it != _playerCrowns.end())
    {
        it->second += crowns;
        CCLOG("Player %d now has %d crowns", playerId, it->second);
    }
}

int BattleManager::getPlayerCrowns(int playerId) const
{
    auto it = _playerCrowns.find(playerId);
    if (it != _playerCrowns.end())
    {
        return it->second;
    }
    return 0;
}

void BattleManager::reset()
{
    _gameActive = false;
    _gameEnded = false;
    _gameTime = 0.0f;
    _currentGameMode = "normal";

    _playerCrowns[1] = 0;
    _playerCrowns[2] = 0;

    if (_manaSystem)
    {
        _manaSystem->reset();
    }

    CCLOG("BattleManager reset");
}

void BattleManager::setGameMode(const std::string& mode)
{
    if (_currentGameMode == mode)
        return;

    _currentGameMode = mode;

    if (mode == "double_elixir")
    {
        _manaSystem->setManaMode(ManaSystem::ManaMode::DOUBLE);
        CCLOG("Game mode changed to: DOUBLE ELIXIR");
    }
    else if (mode == "sudden_death")
    {
        _manaSystem->setManaMode(ManaSystem::ManaMode::SUDDEN_DEATH);
        CCLOG("Game mode changed to: SUDDEN DEATH");
    }
    else
    {
        _manaSystem->setManaMode(ManaSystem::ManaMode::NORMAL);
        CCLOG("Game mode changed to: NORMAL");
    }
}