#ifndef __SKELETON_TROOP_H__
#define __SKELETON_TROOP_H__

#include "TroopBase.h"

// SkeletonTroop
// -------------
// 骷髅士兵，具有近战攻击功能，模拟简单的AI和行动逻辑
//
class SkeletonTroop : public TroopBase
{
public:
    SkeletonTroop();  // 声明构造函数
    ~SkeletonTroop();
    virtual bool init() override;

    CREATE_FUNC(SkeletonTroop);

private:
    // 在构造函数中初始化组件
    void setupComponents();
};

#endif // __SKELETON_TROOP_H__
