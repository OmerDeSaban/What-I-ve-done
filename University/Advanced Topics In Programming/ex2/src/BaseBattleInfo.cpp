#include "BaseBattleInfo.h"

const std::vector<std::vector<char>>& BaseBattleInfo::getGrid() const {
    return grid_;
}

void BaseBattleInfo::setGrid(const std::vector<std::vector<char>>& grid) {
    grid_ = grid;
}

int BaseBattleInfo::getNumShells(){
    return numShells_;
}

void BaseBattleInfo::setNumShells(int ammo){
    numShells_ = ammo;
}