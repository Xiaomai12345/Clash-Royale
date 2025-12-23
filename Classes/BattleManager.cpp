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