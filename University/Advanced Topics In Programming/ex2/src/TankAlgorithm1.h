#pragma once
#include "BaseTankAlgorithm.h"
#include "bfs.h"

class TankAlgorithm1 : public BaseTankAlgorithm {
public:
    TankAlgorithm1(int player_index, int tank_index);

    ActionRequest getAction() override;

    void updateBattleInfo(BattleInfo& info) override;
};