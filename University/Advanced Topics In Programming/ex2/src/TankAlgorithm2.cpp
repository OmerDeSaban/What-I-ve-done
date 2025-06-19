#include "TankAlgorithm2.h"

TankAlgorithm2::TankAlgorithm2(int player_index, int tank_index)
    : BaseTankAlgorithm(player_index, tank_index) {
    dir_ = Direction::R;  // Set direction to R after base construction
}

ActionRequest TankAlgorithm2::getAction() {
    if (!getRelevantInfo()) {
        return ActionRequest::GetBattleInfo;
    }

    setRelevantInfo(false);
    decreaseLastShotCount();

    if (getEnemyPositions().empty()) {
        // No enemies found, fallback action
        cout << "no enemies" << endl;
        return ActionRequest::DoNothing;
    }

    /* Deciding who's the relevant attacker tank, if no line of sight could be ontained from this tile,
     * it'll be the enemy tank at getEnemyPositions()[0]. */
    pair<int, int> attacker_pos = getEnemyPositions()[0];
    vector<pair<pair<int, int>, double>> attackers_in_sight;
    for (const auto& attacker : getEnemyPositions()) {
        // Direction dir = getDirectionToward(attacker);
        if (hasLineOfSight(getPos(), dir_, attacker)) {
            double dis = sqrt(pow(abs(getPos().first - attacker.first), 2) + pow(abs(getPos().second - attacker.second), 2));
            attackers_in_sight.push_back({attacker, dis});
        }
    }
    /* A line of sight could be obtained from this tile unto at least 1 enemy tank, the attacker tank is
     * the closest enemy tank of said tanks: */
    if (!attackers_in_sight.empty()) {
        attacker_pos = attackers_in_sight[0].first;
        double dis = attackers_in_sight[0].second;
        for (const auto& attacker : attackers_in_sight) {
            if (attacker.second < dis) {
                attacker_pos = attacker.first;
                dis = attacker.second;
            }
        }
    }
    /* No enemy tank could be in sight from the current position, the attacker tank is the closest one: */
    else {
        double min_dis = sqrt(pow(abs(getPos().first - attacker_pos.first), 2) + pow(abs(getPos().second - attacker_pos.second), 2));
        for (const auto& attacker : getEnemyPositions()) {
            double dis = sqrt(pow(abs(getPos().first - attacker.first), 2) + pow(abs(getPos().second - attacker.second), 2));
            if (dis < min_dis) {
                min_dis = dis;
                attacker_pos = attacker;
            }
        }
    }

    // Emergency handling if standing on unsafe tile
    if (!isSafe(getPos())) {
        cout << "I'm in danger!" << endl;

        // Try to move forward if safe
        if (canMoveForward()) {
            return ActionRequest::MoveForward;
        }

        // Try all adjacent cells to find a safe one and rotate/move there
        int rows = grid_.size();
        int cols = grid_[0].size();

        for (int i = 0; i < 8; ++i) {
            Direction adj_dir = indexToDirection(i);
            pair<int, int> adj_pos = getNextCoordsInDir(adj_dir, getPos(), rows, cols);

            if (isSafe(adj_pos)) {
                Direction desired_dir = getDirectionToward(adj_pos);
                if (getDir() != desired_dir) {
                    ActionRequest rotation = whichRotation(desired_dir);
                    rotate(rotation);
                    return rotation;
                } else if (canMoveForward()) {
                    return ActionRequest::MoveForward;
                }
            }
        }

        // If cannot move away immediately, try to shoot
        if (canShoot()) {
            decreaseAmmo();
            resetLastShotCount();
            return ActionRequest::Shoot;
        }

        // // Rotate to face attacker for next round shooting
        // Direction desired_dir = getDirectionToward(attacker_pos);
        // if (getDir() != desired_dir) {
        //     ActionRequest rotation = whichRotation(desired_dir);
        //     rotate(rotation);
        //     return rotation;
        // }
    }

    // Normal behavior: if line of sight and can shoot, shoot
    if (hasLineOfSight(getPos(), getDir(), attacker_pos) && canShoot()) {
        decreaseAmmo();
        resetLastShotCount();
        return ActionRequest::Shoot;
    }

    // Try to rotate to get line of sight toward attacker
    ActionRequest rotation = findRotationForLineOfSight(attacker_pos);
    if (rotation != ActionRequest::DoNothing) {
        rotate(rotation);
        return rotation;
    }

    // Default action: move forward or rotate
    if (canMoveForward()){
        return ActionRequest::MoveForward;
    }
    else{
        rotate(ActionRequest::RotateRight90);
        return ActionRequest::RotateRight90;
    }
    
}

void TankAlgorithm2::updateBattleInfo(BattleInfo& info) {
    // Call base version first
    BaseTankAlgorithm::updateBattleInfo(info);
}