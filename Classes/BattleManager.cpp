#include "BattleManager.h"
#include "Battlefield.h"
#include"UnitType.h"
#include"AllCards.h"
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
    ,myKingAlive(true),myLeftPrincessAlive(true),myRightPrincessAlive(true)
    ,enemyKingAlive(true),enemyLeftPrincessAlive(true),enemyRightPrincessAlive(true)
{
    _manaSystem = ManaSystem::getInstance();
    _enemyManaSystem = ManaSystem::getEnemyInstance();
    // 初始化玩家数据
    _playerCrowns[1] = 0;
    _playerCrowns[2] = 0;
    myTower = { true,true,true };
    enemyTower= { true,true,true };
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
    addCrown();
    checkGameEnded();

    if (_gameEnded)
    {
        if (_gameEndCallback)
        {
            _gameEndCallback(_playerCrowns[0]>_playerCrowns[1]?1:2);
        }
        return;
    }
    // 检查游戏模式切换
    if (_gameTime >= 180.0f && _playerCrowns[0] == _playerCrowns[1] && _currentGameMode == "normal")
    {
        _gameTime = 0.0f;
        setGameMode("sudden_death");
    }

    if (_gameTime >= 120.0f )
    {
        if (_currentGameMode == "normal")
        {
            _manaSystem->setManaMode(ManaSystem::ManaMode::DOUBLE);
            _enemyManaSystem->setManaMode(ManaSystem::ManaMode::DOUBLE);
        }
        else if (_currentGameMode == "sudden_death")
        {
            _manaSystem->setManaMode(ManaSystem::ManaMode::TRIPLE);
            _enemyManaSystem->setManaMode(ManaSystem::ManaMode::TRIPLE);
        }
    }

}

void BattleManager::deployUnit(int unitType,
    const Vec2& position,
    int playerId)
{
    if (!_battlefield)
        return;

    Node* unit = nullptr;
    ECamp camp = (playerId == 1) ? ECamp::LEFT : ECamp::RIGHT;

    switch (unitType)
    {
    case UNIT_KNIGHT:
    {
        auto knight = KnightTroop::create();
        unit = knight;
        break;
    }
    case UNIT_ARCHER:
    {
        auto archer = ArcherTroop::create();
        unit = archer;
        break;
    }
    case UNIT_GIANT:
    {
        auto giant = Giant::create();
        unit = giant;
        break;
    }
    case UNIT_VALKYRIE:
    {
        auto valkyrie = ValkyrieTroop::create();
        unit = valkyrie;
        break;
    }
    case UNIT_DRAGONBABY:
    {
        auto dragonbaby = DragonBaby::create();
        unit = dragonbaby;
        break;
    }
    case UNIT_CANNON:
    {
        auto cannon = Cannon::create();
        unit = cannon;
        break;
    }
    case UNIT_SKELETON:
    {
        auto skeleton = SkeletonTroop::create();
        unit = skeleton;
        break;
    }
    case UNIT_MINIONS:
    {
        auto minions = Minions::create();
        unit = minions;
        break;
    }
    case UNIT_SKELETON_TOMBSTONE:
    {
        auto tombstone = SkeletonTombstone::create();
        tombstone->setWorld(_battlefield);
        unit = tombstone;
        break;
    }

    // =========================
    // 特殊单位：骷髅军团（自己管理生成）
    // =========================
    case UNIT_SKELETON_LEGION:
    {
        auto legion = SkeletonLegion::create(6, 20.f, camp);
        legion->setPosition(position);
        _battlefield->addChild(legion);
        legion->setWorld(_battlefield);
        legion->spawnAt(_battlefield, position);
        return; // 
    }

    // =========================
    // 法术类（不属于 Unit）
    // =========================
    case UNIT_FIREBALL:
    {
        auto fireball = FireballSpell::create();
        fireball->cast(position, camp);
        return;
    }
    case UNIT_SLWOWSPELL:
    {
        auto slowDownSpell = SlowDownSpell::create();
        slowDownSpell->cast(position, camp);
        return;
    }

    default:
        return;
    }

    // =========================
    // 通用单位初始化（唯一入口）
    // =========================
    if (unit)
    {
        unit->setPosition(position);

        // === 如果是可战斗单位 ===
        if (auto troop = dynamic_cast<TroopBase*>(unit))
        {
            troop->setCamp(camp);
            troop->setWorld(_battlefield); // 
        }

        if (auto building = dynamic_cast<BuildingBase*>(unit))
        {
            building->setCamp(camp);
        }

        _battlefield->addUnit(unit);
    }
}


void BattleManager::addCrown()
{
    if (myLeftPrincessAlive == false && myTower[0] == true)
    {
        _battlefield->expandEnemyDeployArea(0);
        _playerCrowns[1] += 1;
        myTower[0] = false;
    }
    if (myRightPrincessAlive == false && myTower[1] == true)
    {
        _battlefield->expandEnemyDeployArea(1);
        _playerCrowns[1] += 1;
        myTower[1] = false;
    }
    if (myKingAlive == false && myTower[2] == true)
    {
        _playerCrowns[1] += 3;
        myTower[1] = false;
    }

    if(enemyLeftPrincessAlive==false&&enemyTower[0]==true)
    { 
        _battlefield->expandMyDeployArea(0);
        _playerCrowns[0] += 1;
        enemyTower[0] = false;
    }
    if (enemyRightPrincessAlive == false && enemyTower[1] == true)
    {
        _battlefield->expandMyDeployArea(1);
        _playerCrowns[0] += 1;
        enemyTower[1] = false;
    }
    if (enemyKingAlive == false && enemyTower[2] == true)
    {
        _playerCrowns[0] += 3;
        enemyTower[2] = false;
    }
    _playerCrowns[0] = _playerCrowns[0] >= 3 ? 3 : _playerCrowns[0];
    _playerCrowns[1] = _playerCrowns[1] >= 3 ? 3 : _playerCrowns[1];
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

void BattleManager::checkGameEnded()
{
    if (_currentGameMode == "normal")
    {
        if (_gameTime >= 180.0f && _playerCrowns[0] != _playerCrowns[1])
        {
            _gameEnded = true;
            return;
        }
        if (_playerCrowns[0]==3|| _playerCrowns[1]==3)
        {
            _gameEnded = true;
            return;
        }
    }
    else if (_currentGameMode == "sudden_death")
    {
        if (_playerCrowns[0] != _playerCrowns[1]|| _gameTime >= 180.0f)
        {
            _gameEnded = true;
            return;
        }
    }
}

