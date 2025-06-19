/**
 * @file tankB.cpp
 * @brief A file detailing the implementation of all the functions declared in tankB.h.
 * The maximal number of columns in a comment-line in this file is 106.
 */

#include "tankB.h"
using namespace std;

int TankB::shots_fired_at_b = 0;
int TankB::last_known_ammo = 16;

Action TankB::nextAction(Board& board) {
    pair<int, int> attacker_pos = board.getPosTankA();

    /* Emergency handling if tankB is standing on an unsafe tile: */
    if (!board.isSafe(getPos())) {
        cout << "I'm in danger!" << endl;
        // Try to move to a safe adjacent vertex immediately
        if (canMoveForward(board)) {
            return Action::Forward;
        }

        vector<pair<int, int>> adj;
        for (const auto& pair : board.getVertices()) {
            if (pair.first == getPos()) { adj = pair.second.getAdj(); break; }
        }

        for (const auto& coords : adj) {
            if (board.isSafe(coords)) {
                Direction desired_dir = getDirectionToward(coords, board);

                if (getDir() != desired_dir) {
                    return whichRotation(desired_dir);
                } else {
                    if (canMoveForward(board)) {
                        return Action::Forward;
                    }
                }
            }
        }

        // If cannot move away immediately, try to shoot (maybe at a wall / mine / attacker)
        if (canShoot()) {
            return Action::Shoot;
        }

        // rotate to shoot tankA in next round
        Direction desired_dir = getDirectionToward(attacker_pos, board);
        if (getDir() != desired_dir) {
            return whichRotation(desired_dir);
        }
    }

    /* Normal strategic behavior: */

    // if has line of sight, shoot
    if (hasLineOfSight(getPos(), getDir(), attacker_pos, board) && canShoot()) { return Action::Shoot; }

    /* Try to rotate to find a line of sight toward attacker: */
    Action rotation = findRotationForLineOfSight(attacker_pos, board);
    if (rotation != Action::NoAction) { return rotation; }
    return Action::Forward;
}