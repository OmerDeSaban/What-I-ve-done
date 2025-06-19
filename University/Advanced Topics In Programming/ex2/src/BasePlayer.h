#pragma once
#include "Player.h"
#include "BaseBattleInfo.h"
#include <vector>

class BasePlayer : public Player {
private:
    int playerInd_;
    size_t rows_;
    size_t columns_;
    size_t maxSteps_;
    size_t numShells_;
    int livingTanks_;

public:
    // Constructor
    BasePlayer(int player_index, size_t x, size_t y, size_t max_steps, size_t num_shells);

    // Getter methods
    int getPlayerIndex() const;
    size_t getRows() const;
    size_t getColumns() const;
    size_t getMaxSteps() const;
    size_t getNumShells() const;
    int getLivingTanks() const;

    // Setter methods
    void setPlayerIndex(int index);
    void setRows(size_t rows);
    void setColumns(size_t cols);
    void setMaxSteps(size_t steps);
    void setNumShells(size_t shells);
    void setLivingTanks(int count);
    void increaseLivingTanks();

    // Override from Player interface
    void updateTankWithBattleInfo(TankAlgorithm& tank, SatelliteView& satellite_view) override;
};