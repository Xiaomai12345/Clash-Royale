#include "SkeletonTombstone.h"
#include "SkeletonLegion.h"  // 引入军团生成类
#include "DataManager.h"

USING_NS_CC;

SkeletonTombstone::SkeletonTombstone()
{
    _maxHp = 1000;  // 设置墓碑的最大血量
    _hp = _maxHp;
    _bodyRadius = 30.f;  // 设置墓碑的碰撞半径
    _camp = ECamp::LEFT;  // 默认为中立阵营
    _moveAttacked = MoveAttack::Both;  // 不会被攻击
    _isDying = false;  // 初始化为不死
}

SkeletonTombstone::~SkeletonTombstone()
{
}

bool SkeletonTombstone::init()
{
    if (!BuildingBase::init())  // 调用基类的初始化
        return false;

    // 默认值
    _spawnInterval = 3.0f; 
    float lifeTime = 40.0f; // 默认40秒
    _maxHp = 250;

    // 使用 DataManager 读取数值 (ValueMap 方式，与 ArcherTroop 保持一致)
    auto data = DataManager::getInstance()->getCardDataByName("skeletontombstone");
    if (!data.empty())
    {
        if (data.count("health")) {
            _maxHp = data["health"].asInt();
        }
        if (data.count("attackSpeed")) {
            _spawnInterval = data["attackSpeed"].asFloat();
        }
    }

    _hp = _maxHp;
    

    _damageInterval = 1.0f;
    _damagePerTick = std::ceil((float)_maxHp / lifeTime);

    // 设置墓碑的外观
    _sprite = Sprite::create("Images/buildings/skeleton_tombstone.png");
    if (_sprite)
    {
        addChild(_sprite);
        _sprite->setScale(0.7f);  // 调整缩放
    }
    else
    {
        CCLOG("SkeletonTombstone sprite load failed!");
        return false;
    }

    // 初始化血条
    initHpBar();
    _hpBarInited = true;

    // 定时生成骷髅
    schedule([this](float dt) { spawnSkeletons(dt); }, _spawnInterval, "spawn_skeleton_key");

    // 定时扣血
    schedule([this](float dt) { takeDamageOverTime(dt); }, _damageInterval, "damage_over_time_key");

    return true;
}

void SkeletonTombstone::spawnSkeletons(float dt)
{
    // 确保父节点存在
    Node* parent = getParent();
    if (!parent)
    {
        CCLOG("SkeletonTombstone parent is null, cannot spawn skeletons");
        return;
    }

    // 调用 spawnAt 生成骷髅
    SkeletonLegion* legion = SkeletonLegion::create(_skeletonCount, _spawnRadius, _camp);
    legion->setWorld(_world);
    parent->addChild(legion);
    legion->spawnAt(parent, getPosition());  // 将骷髅生成在墓碑的位置

    CCLOG("Skeletons spawned at (%.1f, %.1f)", getPosition().x, getPosition().y);
}

void SkeletonTombstone::takeDamageOverTime(float dt)
{
    // 每隔一定时间扣除血量
    takeDamage(_damagePerTick);
    CCLOG("Tombstone taking %d damage. Remaining HP: %d/%d", _damagePerTick, _hp, _maxHp);
}
void SkeletonTombstone::setWorld(IWalkableWorld* world)
{
    _world = world;
}
