#ifndef __KNIGHT_TROOP_H__
#define __KNIGHT_TROOP_H__

#include "TroopBase.h"

// KnightTroop
// -----------
// 测试用近战士兵
// 用于验证 AI / Move / Attack 是否能正常工作
//
class KnightTroop : public TroopBase
{
public:
    KnightTroop();  // 声明构造函数
    ~KnightTroop();
    virtual bool init() override;

    CREATE_FUNC(KnightTroop);
};

#endif // __KNIGHT_TROOP_H__
