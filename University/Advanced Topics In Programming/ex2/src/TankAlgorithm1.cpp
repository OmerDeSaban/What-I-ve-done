#include "TankAlgorithm1.h"

TankAlgorithm1::TankAlgorithm1(int player_index, int tank_index)
    : BaseTankAlgorithm(player_index, tank_index) {
    dir_ = Direction::L;  // Set direction to L after base construction
}

ActionRequest TankAlgorithm1::getAction() {
    if (!getRelevantInfo()) {
        return ActionRequest::GetBattleInfo;
    }

    setRelevantInfo(false);
    decreaseLastShotCount();

    if (getEnemyPositions().empty()) {
        cout << "no enemies" << endl;
        return ActionRequest::GetBattleInfo;
    }

    pair<int, int> target_pos = getEnemyPositions()[0];

    // Step 1: Shoot if possible
    if (hasLineOfSight(getPos(), getDir(), target_pos) && canShoot()) {
        decreaseAmmo();
        resetLastShotCount();
        return ActionRequest::Shoot;
    }

    // Step 2: Rotate for line of sight
    ActionRequest rotation = findRotationForLineOfSight(target_pos);
    if (rotation != ActionRequest::DoNothing) {
        rotate(rotation);
        return rotation;
    }

    // Step 3: BFS path to target using grid
    BFS bfs;
    vector<vector<char>> grid = getGrid();
    bfs.initializeBFSVertices(grid);
    bfs.runBFS(getPos(), target_pos, grid);

    const auto& path = bfs.getBFSPath();
    std::cout << "BFS Path: ";
    for (const auto& pos : path) {
        std::cout << "(" << pos.first << "," << pos.second << ") ";
    }
    std::cout << std::endl;
    if (bfs.pathExists() && !bfs.getBFSPath().empty()) {
        if (isPathSafe(bfs.getBFSPath())) {
            pair<int, int> next = getNextPos(bfs.getBFSPath());
            Direction desired_dir = getDirectionToward(next);
            if (getDir() != desired_dir) {
                ActionRequest rotation = whichRotation(desired_dir);
                rotate(rotation);
                return rotation;
            } else if (canMoveForward()) {
                return ActionRequest::MoveForward;
            }
        }
    }

    // Step 4: Try adjacent positions
    std::pair<int, int> current_pos = getPos();
    int rows = grid.size();
    int cols = grid[0].size();

    for (size_t i = 0; i < 8; ++i) {
        std::pair<int, int> adj = getNextCoordsInDir(indexToDirection(i), current_pos, rows, cols);
        if (!isSafe(adj)) continue;
        for (int dir_idx = 0; dir_idx < 8; ++dir_idx) {
            Direction dir = indexToDirection(dir_idx);
            if (hasLineOfSight(adj, dir, target_pos)) {
                Direction move_dir = getDirectionToward(adj);
                if (getDir() != move_dir) {
                    ActionRequest rotation = whichRotation(move_dir);
                    rotate(rotation);
                    return rotation;
                } else if (canMoveForward()) {
                    return ActionRequest::MoveForward;
                }
            }
        }
    }

    // Step 5: Shoot walls by scanning grid
    for (size_t r = 0; r < grid.size(); r++) {
        for (size_t c = 0; c < grid[0].size(); c++) {
            if (grid[r][c] == '#') {
                pair<int, int> wall_pos = {r, c};
                for (int dir_idx = 0; dir_idx < 8; dir_idx++) {
                    Direction dir = indexToDirection(dir_idx);
                    if (hasLineOfSight(getPos(), dir, wall_pos)) {
                        if (dir == getDir() && canShoot()) {
                            decreaseAmmo();
                            resetLastShotCount();
                            return ActionRequest::Shoot;
                        } else if (dir != getDir()) {
                            ActionRequest rotation = whichRotation(dir);
                            rotate(rotation);
                            return rotation;
                        }
                    }
                }
            }
        }
    }

        // Default action: move forward or rotate
    if (canMoveForward()){
        return ActionRequest::MoveForward;
    }
    else{
        rotate(ActionRequest::RotateRight90);
        return ActionRequest::RotateRight90;
    }

    return ActionRequest::MoveForward;
}


void TankAlgorithm1::updateBattleInfo(BattleInfo& info) {
    // Call base version first
    BaseTankAlgorithm::updateBattleInfo(info);

    // Add custom logic
}