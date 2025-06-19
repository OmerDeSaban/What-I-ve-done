#pragma once
#include "BattleInfo.h"
#include <vector>
#include <memory>

class BaseBattleInfo : public BattleInfo {
    private:
        std::vector<std::vector<char>> grid_;
        int numShells_;

    public:
        const std::vector<std::vector<char>>& getGrid() const;
        int getNumShells();
        void setGrid(const std::vector<std::vector<char>>& grid);
        void setNumShells(int ammo);
};