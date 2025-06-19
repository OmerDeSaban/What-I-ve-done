#include "BaseTankAlgorithm.h"

// Constructor
BaseTankAlgorithm::BaseTankAlgorithm(int player_index, int tank_index)
    : player_index_(player_index),
      tank_index_(tank_index),
      dir_(indexToDirection(0)),       // Default to North
      ammo_(-1),
      pos_({-1, -1}),
      relevantInfo_(false),
      grid_(),
      enemy_positions_(),
      lastShotCnt_(0)
{
}

// Getter for player index
int BaseTankAlgorithm::getPlayerInd() const {
    return player_index_;
}

// Default implementation of updateBattleInfo
void BaseTankAlgorithm::updateBattleInfo(BattleInfo& info) {
    relevantInfo_ = true;
    BaseBattleInfo* baseInfo = dynamic_cast<BaseBattleInfo*>(&info);
    if (baseInfo) {
        int num_shells = baseInfo->getNumShells();
        if (getAmmo() == -1){
            setAmmo(num_shells);
        }
        grid_ = baseInfo->getGrid();
        findMyPos();
        findEnemyPositions();
    }
}

void BaseTankAlgorithm::findMyPos() {
    pos_ = {-1, -1}; // Reset position
    for (size_t row = 0; row < grid_.size(); ++row) {
        for (size_t col = 0; col < grid_[row].size(); ++col) {
            if (grid_[row][col] == '%') {
                pos_ = {row, col};
                return; // Exit early when found
            }
        }
    }
    // If '%' is not found, pos_ remains (-1, -1)
}

std::pair<int, int> BaseTankAlgorithm::getNextPos(const std::vector<std::pair<int, int>>& path) {
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

Direction BaseTankAlgorithm::getDirectionToward(pair<int, int> coords) {
    int rows = grid_.size();
    int cols = grid_[0].size();

    for (size_t i = 0; i < 8; ++i) {
        Direction dir = indexToDirection(i);
        pair<int, int> nextCoords = getNextCoordsInDir(dir, pos_, rows, cols);
        if (nextCoords == coords) {
            return dir;
        }
    }

    throw runtime_error("Desired vertex is not adjacent to current position");
}

ActionRequest BaseTankAlgorithm::whichRotation(Direction desired_dir) {
    int currentIdx = directionToIndex(dir_);
    int desiredIdx = directionToIndex(desired_dir);
    int diff = (desiredIdx - currentIdx + 8) % 8;
    if (diff == 0) {
        // Already facing the right direction - Don't act on this!!
        return ActionRequest::RotateLeft45;
    } else if (diff == 1 || diff == 7) {
        return (diff == 1) ? ActionRequest::RotateRight45 : ActionRequest::RotateLeft45;
    } else if (diff == 2 || diff == 6) {
        return (diff == 2) ? ActionRequest::RotateRight90 : ActionRequest::RotateLeft90;
    } else if (diff == 3 || diff == 5) {
        // Pick the more efficient 4-rotation
        return (diff == 3) ? ActionRequest::RotateRight90 : ActionRequest::RotateLeft90;
    } else {
        // Opposite direction, choose either
        return ActionRequest::RotateRight90;
    }
}

bool BaseTankAlgorithm::canMoveForward() {
    int rows = grid_.size();
    int cols = grid_[0].size();

    // Get next position in the current direction
    pair<int, int> next_pos = getNextCoordsInDir(dir_, pos_, rows, cols);

    // Check if safe
    return isSafe(next_pos);
}


bool BaseTankAlgorithm::isSafe(pair<int, int> pos) {
    int rows = grid_.size();
    int cols = grid_[0].size();

    char c = grid_[pos.first][pos.second];

    // Check if cell is an obstacle or a shell
    if (c == '@' || c == '*' || c == '#') {
        return false;  // Wall, mine, or shell
    }

    // Check if a shell might move into this position (next 2 turns)
    for (size_t i = 0; i < 8; ++i) {
        pair<int, int> neighbor1 = getNextCoordsInDir(indexToDirection(i), pos, rows, cols);
        char c1 = grid_[neighbor1.first][neighbor1.second];
        if (c1 == '*') {
            return false;  // Shell could reach here
        }

        pair<int, int> neighbor2 = getNextCoordsInDir(indexToDirection(i), neighbor1, rows, cols);
        char c2 = grid_[neighbor2.first][neighbor2.second];
        if (c2 == '*') {
            return false;  // Shell could reach here
        }
    }
    return true;
}

bool BaseTankAlgorithm::canMoveBackward() {
    int rows = grid_.size();
    int cols = grid_[0].size();

    // Compute the opposite direction
    Direction opposite = rotateRightQuarter(rotateRightQuarter(dir_));

    // Get the position one step back
    pair<int, int> next_pos = getNextCoordsInDir(opposite, pos_, rows, cols);

    // Check if that position is safe
    return isSafe(next_pos);
}

bool BaseTankAlgorithm::hasLineOfSight(pair<int, int> pos, Direction dir, pair<int, int> target_pos) {
    int rows = grid_.size();
    int cols = grid_[0].size();
    int maxSteps = max(rows, cols);
    pair<int, int> current_pos = pos;

    for (int step = 0; step < maxSteps; ++step) {
        // Move to next position in the given direction
        current_pos = getNextCoordsInDir(dir, current_pos, rows, cols);

        if (current_pos == target_pos) {
            return true; // Found the target
        }

        char cell = grid_[current_pos.first][current_pos.second];
        if (cell == '#') {
            return false; // Obstacle (wall) blocks line of sight
        }
    }

    return false; // Didn't reach target within max steps
}


ActionRequest BaseTankAlgorithm::findRotationForLineOfSight(pair<int, int> target_pos) {
    for (int i = 0; i < 8; ++i) {
        Direction candidateDir = indexToDirection(i);
        if (hasLineOfSight(pos_, candidateDir, target_pos)) {
            return whichRotation(candidateDir);
        }
    }

    // No line of sight in any direction; fallback action
    return ActionRequest::DoNothing;
}

void BaseTankAlgorithm::findEnemyPositions() {
    enemy_positions_.clear();  // Clear previous data
    for (size_t row = 0; row < grid_.size(); ++row) {
        for (size_t col = 0; col < grid_[row].size(); ++col) {
            char cell = grid_[row][col];
            if (isdigit(cell)) {
                int tankNum = cell - '0';
                if (tankNum != player_index_) {
                    enemy_positions_.emplace_back(row, col);
                }
            }
        }
    }
}

void BaseTankAlgorithm::rotate(ActionRequest action){
    Direction newDirection = getDir();

    if (action == ActionRequest::RotateLeft90) {
        newDirection = rotateLeftQuarter(getDir());
    } else if (action == ActionRequest::RotateRight90) {
        newDirection = rotateRightQuarter(getDir());
    } else if (action == ActionRequest::RotateLeft45) {
        newDirection = rotateLeftEighth(getDir());
    } else if (action == ActionRequest::RotateRight45) {
        newDirection = rotateRightEighth(getDir());
    }
    setDir(newDirection);
}

bool BaseTankAlgorithm::isPathSafe(const std::vector<std::pair<int, int>>& path) {
    for (const auto& pos : path) {
        if (!isSafe(pos)) {
            return false;  // If any part of the path is unsafe, the whole path is unsafe
        }
    }
    return true;  // All positions are safe
}