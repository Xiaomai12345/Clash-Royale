#ifndef __ARCHER_TROOP_H__
#define __ARCHER_TROOP_H__

#include "TroopBase.h"

// ArcherTroop
// -----------
// 测试用远程士兵（弓箭手）
//
// 用于验证：
// - 远程攻击组件
// - 攻击距离停靠逻辑
// - AI / Move / Attack 协作
//
class ArcherTroop : public TroopBase
{
public:
    virtual bool init() override;

    CREATE_FUNC(ArcherTroop);
};

#endif // __ARCHER_TROOP_H__
