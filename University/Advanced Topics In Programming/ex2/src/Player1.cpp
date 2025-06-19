#include "Player1.h"

Player1::Player1(int player_index, size_t x, size_t y, size_t max_steps, size_t num_shells)
    : BasePlayer(player_index, x, y, max_steps, num_shells), tankInd_(0), targetedEnemy_(false),
    targetPos_() { }

Player1::~Player1() = default;

void Player1::updateTankWithBattleInfo(TankAlgorithm& tank, SatelliteView& satellite_view) {
    std::vector<std::vector<char>> grid(getRows(), std::vector<char>(getColumns()));

    /* Getting a new target: */
    // if (true) {
    std::vector<std::pair<size_t, size_t>> allied_coords;
    std::vector<std::pair<size_t, size_t>> enemy_coords;

    for (size_t y = 0; y < getRows(); ++y) {
        for (size_t x = 0; x < getColumns(); ++x) {
            grid[y][x] = satellite_view.getObjectAt(x, y);
            if (grid[y][x] == '1' || grid[y][x] == '%') { allied_coords.push_back(std::pair(y, x)); }
            else if (grid[y][x] == '2') { enemy_coords.push_back(std::pair(y, x)); }
        }
    }

    /* Finding the closest enemy tank (tile-wise, regardless of obstacles) to an allied tank: */
    double min_dis = getRows() + getColumns() + 1;
    for (const auto& ally : allied_coords) {
        for (const auto& enemy : enemy_coords) {
            double dis = sqrt(pow(abs(ally.first - enemy.first), 2) + pow(abs(ally.second - enemy.second), 2));
            if (dis < min_dis) {
                min_dis = abs(ally.first - enemy.first) + abs(ally.second - enemy.second);
                targetedEnemy_ = true;
                targetPos_ = enemy;
            }
        }
    }

    /* Ensuring the tank would only see the desired target: */
    for (const auto& enemy : enemy_coords) {
        if (enemy != targetPos_) { 
            grid[enemy.first][enemy.second] = ' ';
        }
    }

    BaseBattleInfo battleInfo;
    battleInfo.setGrid(grid);
    battleInfo.setNumShells(getNumShells());

    tank.updateBattleInfo(battleInfo);
}