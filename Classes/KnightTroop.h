#ifndef __KNIGHT_TROOP_H__
#define __KNIGHT_TROOP_H__

#include "TroopBase.h"

class KnightTroop : public TroopBase
{
public:
    KnightTroop();  // 声明构造函数
    ~KnightTroop();
    virtual bool init() override;
    virtual void update(float dt) override; // 重写 update 以处理旋转

    CREATE_FUNC(KnightTroop);
};

#endif // __KNIGHT_TROOP_H__
