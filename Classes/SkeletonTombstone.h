#ifndef __SKELETON_TOMBSTONE_H__
#define __SKELETON_TOMBSTONE_H__

#include "BuildingBase.h"
#include "SkeletonLegion.h"  // 引入军团生成类

class SkeletonTombstone : public BuildingBase
{
public:
    SkeletonTombstone();
    virtual ~SkeletonTombstone();

    virtual bool init() override;  // 初始化

    CREATE_FUNC(SkeletonTombstone); // 用于创建实例

protected:
    void spawnSkeletons(float dt);  // 定时生成骷髅
    void takeDamageOverTime(float dt);  // 定时扣血

private:
    float _damageInterval = 3.0f;  // 扣血时间间隔
    int _damagePerTick = 5;        // 每次扣血的量
    float _spawnInterval = 5.0f;   // 生成骷髅的间隔
    int _skeletonCount = 1;        // 每次生成的骷髅数
    float _spawnRadius = 50.f;     // 骷髅生成的散布半径
};

#endif // __SKELETON_TOMBSTONE_H__
