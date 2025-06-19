/**
 * @file tankB.h
 * @brief A file depicting the TankB class declaration.
 * The maximal number of columns in a comment-line in this file is 106.
 */

#pragma once
#include "tank.h"

class TankB : public Tank {
    private:
        static int shots_fired_at_b;
        static int last_known_ammo;
    public:
        TankB() : Tank() { }
        TankB(std::pair<int, int> pos, Direction dir) : Tank(pos, dir) { }
        Action nextAction(Board& board);
};