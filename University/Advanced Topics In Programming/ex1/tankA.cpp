/**
 * @file tankA.cpp
 * @brief A file detailing the implementation of all the functions declared in tankA.h.
 * The maximal number of columns in a comment-line in this file is 106.
 */

#include "tankA.h"
using namespace std;

Action TankA::nextAction(Board& board) {
    pair<int, int> target_pos = board.getPosTankB();
    // Step 1: If tankA can shoot directly at tankB, do it:
    if (hasLineOfSight(getPos(), getDir(), target_pos, board) && canShoot()) { return Action::Shoot; }
    // Step 2: Rotate to get line of sight on tankB:
    for (int i = 0; i < 8; i++) {
        Direction candidate_dir = indexToDirection(i);
        if (getDir() != candidate_dir && hasLineOfSight(getPos(), candidate_dir, target_pos, board) &&
            canShoot()) { return whichRotation(candidate_dir); }
    }
    // Update BFS data if needed (this is Step 3.-1 in some sense):
    if (board.getBFS().getBFSVertices().empty()) {
        BFS bfs = board.getBFS();
        for (int i = 0; i < board.getRows(); i++) {
            for (int j = 0; j < board.getColumns(); j++) {
                if (board.getGrid()[i][j] == "#") { board.addNewVertexToMap(i, j, true, false); }
                else if (board.getGrid()[i][j] == "@") { board.addNewVertexToMap(i, j, false, true); }
                else { board.addNewVertexToMap(i, j); }
                BFS_Vertex bfs_vertex({i, j});
                bfs.addBFSVertex(bfs_vertex);
            }
        }
        bfs.runBFS(getPos(), target_pos, board.getVertices());
        board.setBFS(bfs);
    }
    else if (board.getBFS().getV() != target_pos || !board.getBFS().isOnPath(getPos())) {
        BFS bfs = board.getBFS();
        bfs.runBFS(getPos(), board.getPosTankB(), board.getVertices());
        board.setBFS(bfs);
    }
    // Step 3: Follow BFS path if one exists:
    vector<pair<int, int>> path_to_target = board.getBFS().getBFSPath();
    bool path_exists = board.getBFS().pathExists();
    if (path_exists && path_to_target.empty()) {
        throw invalid_argument("With given board's state, got an existance of an empty path between the tanks");
    }
    else if (path_exists) {
        if (board.isBFSPathSafe()) {
            pair<int, int> desiredNext = getNextPos(path_to_target);
            Direction desired_dir = getDirectionToward(desiredNext, board);
            if (getDir() != desired_dir) { return whichRotation(desired_dir); }
            else if (canMoveForward(board)) { cout << "path_exists" << endl;
                return Action::Forward; }
        }
    }
    // Step 4: Try to move to get line of sight:
    vector<pair<int, int>> adj;
    for (const auto& pair : board.getVertices()) {
        if (pair.first == getPos()) {
            adj = pair.second.getAdj();
            break;
        }
    }
    for (size_t i = 0; i < adj.size(); i++) {
        pair<int, int> adj_pos = adj[i];
        if (!board.isSafe(adj_pos)) { continue; }
        for (int dir_idx = 0; dir_idx < 8; dir_idx++) {
            Direction check_dir = indexToDirection(dir_idx);
            if (hasLineOfSight(adj_pos, check_dir, target_pos, board)) {
                Direction move_dir = indexToDirection(i);
                if (getDir() != move_dir) { return whichRotation(move_dir); }
                else if (canMoveForward(board)) { cout << "step 4" << endl;
                    return Action::Forward; }
            }
        }
    }
    // Step 5: Try to destroy walls that block line of sight:
    for (const auto& vertex_pair : board.getVertices()) {
        const Vertex& vertex = vertex_pair.second;
        if (vertex.isWall()) {
            pair<int, int> wall_pos = vertex.getCoords();
            bool wall_blocks = false;
            for (int dir_idx = 0; dir_idx < 8; dir_idx++) {
                Direction check_dir = indexToDirection(dir_idx);
                if (hasLineOfSight(getPos(), check_dir, wall_pos, board)) {
                    if (check_dir == getDir() && canShoot()) { return Action::Shoot; }
                    if (check_dir != getDir()) { return whichRotation(check_dir); }
                    wall_blocks = true;
                    break;
                }
            }
            if (wall_blocks) {
                for (size_t i = 0; i < adj.size(); i++) {
                    pair<int, int> adj_pos = adj[i];
                    if (!board.isSafe(adj_pos)) { continue; }
                    for (int dir_idx = 0; dir_idx < 8; dir_idx++) {
                        Direction check_dir = indexToDirection(dir_idx);
                        if (hasLineOfSight(adj_pos, check_dir, wall_pos, board)) {
                            Direction move_dir = indexToDirection(i);
                            if (getDir() != move_dir) { return whichRotation(move_dir); }
                            else if (canMoveForward(board)) { cout << "wall_blocks" << endl;
                                return Action::Forward; }
                        }
                    }
                }
            }
        }
    }
    // Step 6: Find nearest mine and move towards it (trying to avoid endless games):
    pair<int, int> nearest_mine = {-1, -1};
    int min_distance = numeric_limits<int>::max();
    for (const auto& vertex_pair : board.getVertices()) {
        const Vertex& vertex = vertex_pair.second;
        if (vertex.isMine()) {
            pair<int, int> mine_pos = vertex.getCoords();
            int distance = abs(mine_pos.first - getPos().first) + abs(mine_pos.second - getPos().second);
            if (distance < min_distance) {
                min_distance = distance;
                nearest_mine = mine_pos;
            }
        }
    }
    if (nearest_mine.first != -1) {
        BFS mine_bfs = board.getBFS();
        mine_bfs.runBFS(getPos(), nearest_mine, board.getVertices());
        if (mine_bfs.pathExists() && !mine_bfs.getBFSPath().empty()) {
            vector<pair<int, int>> path_to_mine = mine_bfs.getBFSPath();
            pair<int, int> next_pos = getNextPos(path_to_mine);
            Direction move_dir = getDirectionToward(next_pos, board);
            if (getDir() != move_dir) { return whichRotation(move_dir); }
            else if (canMoveForward(board)) { cout << "suicide" << endl;
                return Action::Forward; }
        }
    }
    // Sted 7: If all else fails, commence turret-nado!
    return Action::Rotate8Right;
}