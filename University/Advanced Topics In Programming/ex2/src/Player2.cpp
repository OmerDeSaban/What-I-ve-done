#include "Player2.h"

Player2::Player2(int player_index, size_t x, size_t y, size_t max_steps, size_t num_shells)
    : BasePlayer(player_index, x, y, max_steps, num_shells) {
}

Player2::~Player2() = default;

void Player2::updateTankWithBattleInfo(TankAlgorithm& tank, SatelliteView& satellite_view) {
    BasePlayer::updateTankWithBattleInfo(tank, satellite_view);
}