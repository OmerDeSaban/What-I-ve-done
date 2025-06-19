/**
 * @file tankA.h
 * @brief A file depicting the TankA class declaration.
 * The maximal number of columns in a comment-line in this file is 106.
 */

#pragma once
#include "tank.h"

class TankA : public Tank {
public:
    TankA() : Tank() { }
    TankA(std::pair<int, int> pos, Direction dir) : Tank(pos, dir) { }
    Action nextAction(Board& board);
};