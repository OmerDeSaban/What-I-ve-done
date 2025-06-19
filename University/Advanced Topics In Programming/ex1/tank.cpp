/**
 * @file tank.cpp
 * @brief A file detailing the implementation of all the functions declared in tank.h.
 * The maximal number of columns in a comment-line in this file is 106.
 */

#include "tank.h"
using namespace std;

/* ------------------------------------------ CONSTRUCTORS: ------------------------------------------ */

Tank::Tank() {
    pos_ = {0, 0};
    dir_ = Direction::U;
    ammo_ = 16;
    backCnt_ = -1;
    lastShotCnt_ = 0;
    alive_ = true;
    steppedOnAMine_ = false;
    hitByAShell_ = false;
    crashedWithOtherTank_ = false;
}

Tank::Tank(pair<int, int> pos, Direction dir, int ammo, int back_cnt, int last_shot_cnt, bool alive,
    bool stepped_on_a_mine, bool hit_by_a_shell, bool crashed_with_other_tank) {
        pos_ = pos;
        dir_ = dir;
        ammo_ = ammo;
        backCnt_ = back_cnt;
        lastShotCnt_ = last_shot_cnt;
        alive_ = alive;
        steppedOnAMine_ = stepped_on_a_mine;
        hitByAShell_ = hit_by_a_shell;
        crashedWithOtherTank_ = crashed_with_other_tank;
    }

Tank::Tank(const Tank& tank) {
    pos_ = tank.getPos();
    dir_ = tank.getDir();
    ammo_ = tank.getAmmo();
    backCnt_ = tank.getBackCnt();
    lastShotCnt_ = tank.getLastShotCnt();
    alive_ = tank.alive();
    steppedOnAMine_ = tank.steppedOnAMine();
    hitByAShell_ = tank.hitByAShell();
    crashedWithOtherTank_ = tank.crashedWithOtherTank();
}

Tank::Tank(Tank&& tank) noexcept {
    pos_ = tank.getPos();
    dir_ = tank.getDir();
    ammo_ = tank.getAmmo();
    backCnt_ = tank.getBackCnt();
    lastShotCnt_ = tank.getLastShotCnt();
    alive_ = tank.alive();
    steppedOnAMine_ = tank.steppedOnAMine();
    hitByAShell_ = tank.hitByAShell();
    crashedWithOtherTank_ = tank.crashedWithOtherTank();
    tank.resetTank();
}

/* -------------------------------------------- OPERATORS: ------------------------------------------- */

Tank& Tank::operator=(const Tank& tank) {
    if (this != &tank) {
        pos_ = tank.getPos();
        dir_ = tank.getDir();
        ammo_ = tank.getAmmo();
        backCnt_ = tank.getBackCnt();
        lastShotCnt_ = tank.getLastShotCnt();
        alive_ = tank.alive();
        steppedOnAMine_ = tank.steppedOnAMine();
        hitByAShell_ = tank.hitByAShell();
        crashedWithOtherTank_ = tank.crashedWithOtherTank();
    }
    return *this;
}

Tank& Tank::operator=(Tank&& tank) noexcept {
    if (this != &tank) {
        pos_ = tank.getPos();
        dir_ = tank.getDir();
        ammo_ = tank.getAmmo();
        backCnt_ = tank.getBackCnt();
        lastShotCnt_ = tank.getLastShotCnt();
        alive_ = tank.alive();
        steppedOnAMine_ = tank.steppedOnAMine();
        hitByAShell_ = tank.hitByAShell();
        crashedWithOtherTank_ = tank.crashedWithOtherTank();
        tank.resetTank();
    }
    return *this;
}

bool Tank::operator==(const Tank& tank) const {
    return (pos_ == tank.getPos() && dir_ == tank.getDir() && ammo_ == tank.getAmmo() &&
        backCnt_ == tank.getBackCnt() && lastShotCnt_ == tank.getLastShotCnt() &&
        alive_ == tank.alive() && steppedOnAMine_ == tank.steppedOnAMine() &&
        hitByAShell_ == tank.hitByAShell() && crashedWithOtherTank_ == tank.crashedWithOtherTank());
}

bool Tank::operator!=(const Tank& tank) const {
    return !(*this == tank);
}

/* --------------------------------------------- GETTERS: -------------------------------------------- */

pair<int, int> Tank::getPos() const {
    return pos_;
}

Direction Tank::getDir() const {
    return dir_;
}

int Tank::getAmmo() const {
    return ammo_;
}

int Tank::getBackCnt() const {
    return backCnt_;
}

int Tank::getLastShotCnt() const {
    return lastShotCnt_;
}

bool Tank::alive() const {
    return alive_;
}

bool Tank::steppedOnAMine() const {
    return steppedOnAMine_;
}

bool Tank::hitByAShell() const {
    return hitByAShell_;
}

bool Tank::crashedWithOtherTank() const {
    return crashedWithOtherTank_;
}

/* --------------------------------------------- SETTERS: -------------------------------------------- */

void Tank::setPos(pair<int, int> pos) {
    pos_ = pos;
}

void Tank::setDir(Direction dir) {
    dir_ = dir;
}

void Tank::setAmmo(int ammo) {
    ammo_ = ammo;
}

void Tank::setBackCnt(int back_cnt) {
    backCnt_ = back_cnt;
}

void Tank::setLastShotCnt(int last_shot_cnt) {
    lastShotCnt_ = last_shot_cnt;
}

void Tank::setAlive(bool alive) {
    alive_ = alive;
}

void Tank::setSteppedOnAMine(bool stepped_on_a_mine) {
    steppedOnAMine_ = stepped_on_a_mine;
}

void Tank::setHitByAShell(bool hit_by_a_shell) {
    hitByAShell_ = hit_by_a_shell;
}

void Tank::setCrashedWithOtherTank(bool crashed_with_other_tank) {
    crashedWithOtherTank_ = crashed_with_other_tank;
}

/* ------------------------------------- OTHER FUNCTIONS: ------------------------------------ */

void Tank::decreaseAmmo() {
    ammo_ -= 1;
}

void Tank::decreaseBackCnt() {
    backCnt_ -= 1;
}

void Tank::resetBackCnt() {
    backCnt_ = -1;
}

void Tank::decreaseLastShotCount() {
    if (lastShotCnt_ > 0){
        lastShotCnt_ -= 1;
    }
}

void Tank::resetLastShotCnt() {
    lastShotCnt_ = 0;
}

void Tank::KILL() {
    alive_ = false;
}

void Tank::stepOnAMine() {
    steppedOnAMine_ = true;
    alive_ = false;
}

void Tank::kaboom() {
    hitByAShell_ = true;
    alive_ = false;
}

void Tank::batteringRam() {
    crashedWithOtherTank_ = true;
    alive_ = false;
}

string Tank::getCauseOfDeath() {
    string causeOfDeath = "cause of death: ";
    if (steppedOnAMine_) {
        causeOfDeath += "stepped on a mine, ";
    }
    if(hitByAShell_) {
        causeOfDeath += "hit by a shell, ";
    }
    if(crashedWithOtherTank_) {
        causeOfDeath += "crashed with the other tank.";
    }
    return causeOfDeath;
}

pair<int, int> Tank::getNextPos(const vector<pair<int, int>>& path) {
    for (size_t i = 0; i < path.size(); ++i) {
        if (path[i] == pos_) {
            if (i + 1 < path.size()) {
                return path[i + 1];
            } else {
                return path[i];  // Already at the last step
            }
        }
    }
    // If currentPos not found in path, return currentPos (or consider throwing)
    return pos_;
}

Direction Tank::getDirectionToward(pair<int, int> coords, Board& board) {
    vector<pair<int, int>> adj;
    for (const auto& pair : board.getVertices()) { if (pair.first == pos_) { adj = pair.second.getAdj(); } }
    for (size_t i = 0; i < 8; ++i) { if (adj[i] == coords) { return indexToDirection(i); } }
    throw runtime_error("Desired vertex is not adjacent to current position");
}

Action Tank::whichRotation(Direction desired_dir) {
    int currentIdx = directionToIndex(dir_);
    int desiredIdx = directionToIndex(desired_dir);
    int diff = (desiredIdx - currentIdx + 8) % 8;
    if (diff == 0) {
        // Already facing the right direction - Don't act on this!!
        return Action::Rotate8Left;
    } else if (diff == 1 || diff == 7) {
        return (diff == 1) ? Action::Rotate8Right : Action::Rotate8Left;
    } else if (diff == 2 || diff == 6) {
        return (diff == 2) ? Action::Rotate4Right : Action::Rotate4Left;
    } else if (diff == 3 || diff == 5) {
        // Pick the more efficient 4-rotation
        return (diff == 3) ? Action::Rotate4Right : Action::Rotate4Left;
    } else {
        // Opposite direction, choose either
        return Action::Rotate4Right;
    }
}

bool Tank::canMoveForward(Board& board) {
    pair<int, int> next_pos;
    for (const auto& pair : board.getVertices()) {
        if (pair.first == pos_) {
            next_pos = pair.second.getAdjCoordInDirection(dir_);
            break;
        }
    }
    return board.isSafe(next_pos);
}

bool Tank::canMoveBackward(Board& board) {
    Direction opposite = rotateRightQuarter(rotateRightQuarter(dir_));
    pair<int, int> next_pos;
    for (const auto& pair : board.getVertices()) {
        if (pair.first == pos_) {
            next_pos = pair.second.getAdjCoordInDirection(opposite);
            break;
        }
    }
    return board.isSafe(next_pos);
}

bool Tank::hasLineOfSight(pair<int, int> pos, Direction dir, pair<int, int> target_pos, Board& board) {
    int maxSteps = max(board.getRows(), board.getColumns());
    pair<int, int> current_pos = pos;
    for (int step = 0; step < maxSteps; ++step) {
        for (const auto& pair : board.getVertices()) {
            if (pair.first == current_pos) {
                current_pos = pair.second.getAdjCoordInDirection(dir);
                break;
            }
        }

        if (current_pos == target_pos) {
            return true; // Found the target
        }

        if (board.getGrid()[current_pos.first][current_pos.second] == "#") {
                return false; // Obstacle in the way, but not by a mine
            }
    }

    return false; // Didn't reach the target within the step limit
}

bool Tank::canShoot(){
    return (ammo_ > 0 && lastShotCnt_ == 0);
}

Action Tank::findRotationForLineOfSight(pair<int, int> target_pos, Board& board) {
    for (int i = 0; i < 8; ++i) {
        Direction candidateDir = indexToDirection(i);
        if (hasLineOfSight(pos_, candidateDir, target_pos, board)) {
            return whichRotation(candidateDir);
        }
    }

    // No line of sight in any direction; fallback action
    return Action::NoAction;
}

bool Tank::move(Action action, Board& board) {
    pair<int, int> next_pos;
    if (action == Action::Forward){
        if (backCnt_ > 0) {
            // Cancel backward
            resetBackCnt();
            return true;
        }
        for (const auto& pair : board.getVertices()) {
            if (pair.first == pos_) {
                next_pos = pair.second.getAdjCoordInDirection(dir_);
                break;
            }
        }
        setPos(next_pos);
    } else if (action == Action::Backward){
        if (backCnt_ == -1) {
            // Start new backward wait
            backCnt_ = 2;
        } else if (backCnt_ == 0) {
            // Execute backward
            Direction opposite = rotateRightQuarter(rotateRightQuarter(dir_));
            for (const auto& pair : board.getVertices()) {
                if (pair.first == pos_) {
                    next_pos = pair.second.getAdjCoordInDirection(opposite);
                    break;
                }
            }
            setPos(next_pos);
            backCnt_ = 0;  // Enable chaining
        }
    }
    return true;
}

bool Tank::shoot() {
    if (ammo_ <= 0 || lastShotCnt_ > 0) {
        return false;
    }
    decreaseAmmo();
    lastShotCnt_ = 4;
    return true;
}

bool Tank::rotate(Action action) {
    Direction newDirection;
    if (action == Action::Rotate4Left) { newDirection = rotateLeftQuarter(dir_); }
    else if(action == Action::Rotate4Right) { newDirection = rotateRightQuarter(dir_); }
    else if(action == Action::Rotate8Left) { newDirection = rotateLeftEighth(dir_); }
    else if(action == Action::Rotate8Right) { newDirection = rotateRightEighth(dir_); }
    
    setDir(newDirection);
    return true;
}

bool Tank::shouldWait(Action action) {
    return backCnt_ > 0 && !(action == Action::Forward || action == Action::Backward);
}

bool Tank::act(Action action, Board& board) {
    decreaseLastShotCount();
    if (shouldWait(action)){
        decreaseBackCnt();
        return false;
    }

    bool success;

    if (action == Action::Forward || action == Action::Backward) { success = move(action, board); }
    else if (action == Action::Rotate4Left || action == Action::Rotate4Right ||
                action == Action::Rotate8Left || action == Action::Rotate8Right) { success = rotate(action); }
    else if (action == Action::Shoot) { success = shoot(); }

    return success;
}

void Tank::resetTank() {
    pos_ = pair<int, int>();
    dir_ = Direction::U;
    ammo_ = 0;
    backCnt_ = 0;
    lastShotCnt_ = 0;
    alive_ = false;
    steppedOnAMine_ = false;
    hitByAShell_ = false;
    crashedWithOtherTank_ = false;
}