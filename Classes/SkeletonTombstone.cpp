#include "SkeletonTombstone.h"
#include "SkeletonLegion.h"  // 引入军团生成类

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

    // 设置墓碑的外观
    _sprite = Sprite::create("Images/buildings/skeleton_tombstone.jpg");
    if (_sprite)
    {
        addChild(_sprite);
        _sprite->setScale(0.3f);  // 调整缩放
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
