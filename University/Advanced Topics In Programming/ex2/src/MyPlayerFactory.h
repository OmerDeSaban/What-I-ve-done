#pragma once
#include "PlayerFactory.h"
#include "Player1.h"
#include "Player2.h"
#include <memory>

using std::unique_ptr;
using std::make_unique;

class MyPlayerFactory : public PlayerFactory {
public:
    unique_ptr<Player> create(int player_index, size_t x, size_t y, size_t max_steps, size_t num_shells) const override {
        if (player_index == 1) {
            return make_unique<Player1>(player_index, x, y, max_steps, num_shells);
        } else {
            return make_unique<Player2>(player_index, x, y, max_steps, num_shells);
        }
    }
};