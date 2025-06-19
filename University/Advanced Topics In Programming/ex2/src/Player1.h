#pragma once
#include "BasePlayer.h"
#include <cmath>

class Player1 : public BasePlayer {
    private:
        int tankInd_;
        bool targetedEnemy_;
        std::pair<size_t, size_t> targetPos_;

    public:
        Player1(int player_index, size_t x, size_t y, size_t max_steps, size_t num_shells);
        ~Player1() override;
        void updateTankWithBattleInfo(TankAlgorithm& tank, SatelliteView& satellite_view) override;
};