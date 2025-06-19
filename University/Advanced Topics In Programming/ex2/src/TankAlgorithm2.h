#pragma once
#include "BaseTankAlgorithm.h"
#include <cmath>

class TankAlgorithm2 : public BaseTankAlgorithm {
public:
    TankAlgorithm2(int player_index, int tank_index);

    ActionRequest getAction() override;

    void updateBattleInfo(BattleInfo& info) override;
};