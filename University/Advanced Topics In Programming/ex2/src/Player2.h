#pragma once
#include "BasePlayer.h"

class Player2 : public BasePlayer {
public:
    Player2(int player_index, size_t x, size_t y, size_t max_steps, size_t num_shells);
    ~Player2() override;
    void updateTankWithBattleInfo(TankAlgorithm& tank, SatelliteView& satellite_view) override;
};