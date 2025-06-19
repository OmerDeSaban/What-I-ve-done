#pragma once
#include "TankAlgorithmFactory.h"
#include "TankAlgorithm1.h"
#include "TankAlgorithm2.h"
#include <memory>

using std::unique_ptr;
using std::make_unique;

class MyTankAlgorithmFactory : public TankAlgorithmFactory {
public:
    unique_ptr<TankAlgorithm> create(int player_index, int tank_index) const override {
        if (player_index == 1) {
            return make_unique<TankAlgorithm1>(player_index, tank_index);
        } else {
            return make_unique<TankAlgorithm2>(player_index, tank_index);
        }
    }
};