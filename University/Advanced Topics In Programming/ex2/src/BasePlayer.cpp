#include "BasePlayer.h"
#include "TankAlgorithm.h"
#include "SatelliteView.h"

BasePlayer::BasePlayer(int player_index, size_t x, size_t y, size_t max_steps, size_t num_shells)
    : Player(player_index, x, y, max_steps, num_shells),
      playerInd_(player_index),
      rows_(x),
      columns_(y),
      maxSteps_(max_steps),
      numShells_(num_shells),
      livingTanks_(0) {}

// Getters
int BasePlayer::getPlayerIndex() const {
    return playerInd_;
}

size_t BasePlayer::getRows() const {
    return rows_;
}

size_t BasePlayer::getColumns() const {
    return columns_;
}

size_t BasePlayer::getMaxSteps() const {
    return maxSteps_;
}

size_t BasePlayer::getNumShells() const {
    return numShells_;
}

int BasePlayer::getLivingTanks() const {
    return livingTanks_;
}

// Setters
void BasePlayer::setPlayerIndex(int index) {
    playerInd_ = index;
}

void BasePlayer::setRows(size_t rows) {
    rows_ = rows;
}

void BasePlayer::setColumns(size_t cols) {
    columns_ = cols;
}

void BasePlayer::setMaxSteps(size_t steps) {
    maxSteps_ = steps;
}

void BasePlayer::setNumShells(size_t shells) {
    numShells_ = shells;
}

void BasePlayer::setLivingTanks(int count) {
    livingTanks_ = count;
}

void BasePlayer::increaseLivingTanks() {
    livingTanks_ += 1;
}

void BasePlayer::updateTankWithBattleInfo(TankAlgorithm& tank, SatelliteView& satellite_view) {
    std::vector<std::vector<char>> grid(rows_, std::vector<char>(columns_));  // <-- fixed typo here

    for (size_t y = 0; y < rows_; ++y) {
        for (size_t x = 0; x < columns_; ++x) {
            grid[y][x] = satellite_view.getObjectAt(x, y);
        }
    }

    BaseBattleInfo battleInfo;
    battleInfo.setGrid(grid);
    battleInfo.setNumShells(numShells_);

    tank.updateBattleInfo(battleInfo);
}