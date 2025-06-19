/**
 * @file bfs.cpp
 * @brief A file detailing the implementation of all the functions declared in bfs.h.
 * The maximal number of columns in a comment-line in this file is 106.
 */

#include "bfs.h"
#include <algorithm>
using namespace std;

/* ------------------------------------------ CONSTRUCTORS: ------------------------------------------ */

BFS::BFS() {
    BFSPath_ = vector<pair<int, int>>();
    BFSVertices_ = vector<BFS_Vertex>();
    Q_ = vector<BFS_Vertex>();
    S_ = BFS_Vertex();
    V_ = BFS_Vertex();
    pathExists_ = false;
}

BFS::BFS(int spaces) {
    BFSPath_ = vector<pair<int, int>>();
    BFSPath_.reserve(spaces);
    BFSVertices_ = vector<BFS_Vertex>();
    BFSVertices_.reserve(spaces);
    Q_ = vector<BFS_Vertex>();
    Q_.reserve(spaces);
    S_ = BFS_Vertex();
    V_ = BFS_Vertex();
    pathExists_ = false;
}

BFS::BFS(const vector<pair<int, int>> bfs_path, const vector<BFS_Vertex> bfs_vertices,
    const vector<BFS_Vertex> Q, BFS_Vertex S, BFS_Vertex V, bool path_exists) {
        BFSPath_ = bfs_path;
        BFSVertices_ = bfs_vertices;
        Q_ = Q;
        S_ = S;
        V_ = V;
        pathExists_ = path_exists;
    }

BFS::BFS(const BFS& bfs) {
    BFSPath_ = bfs.getBFSPath();
    BFSVertices_ = bfs.getBFSVertices();
    Q_ = bfs.getQ();
    S_ = bfs.getS();
    V_ = bfs.getV();
    pathExists_ = bfs.pathExists();
}

BFS::BFS(BFS&& bfs) noexcept {
    BFSPath_ = bfs.getBFSPath();
    BFSVertices_ = bfs.getBFSVertices();
    Q_ = bfs.getQ();
    S_ = bfs.getS();
    V_ = bfs.getV();
    pathExists_ = bfs.pathExists();
    bfs.resetBFS();
}

/* -------------------------------------------- OPERATORS: ------------------------------------------- */

BFS& BFS::operator=(const BFS& bfs) {
    if (this != &bfs) {
        BFSPath_ = bfs.getBFSPath();
        BFSVertices_ = bfs.getBFSVertices();
        Q_ = bfs.getQ();
        S_ = bfs.getS();
        V_ = bfs.getV();
        pathExists_ = bfs.pathExists();
    }
    return *this;
}

BFS& BFS::operator=(BFS&& bfs) noexcept {
    if (this != &bfs) {
        BFSPath_ = bfs.getBFSPath();
        BFSVertices_ = bfs.getBFSVertices();
        Q_ = bfs.getQ();
        S_ = bfs.getS();
        V_ = bfs.getV();
        pathExists_ = bfs.pathExists();
        bfs.resetBFS();
    }
    return *this;
}

bool BFS::operator==(const BFS& bfs) const {
    return (BFSPath_ == bfs.getBFSPath() && BFSVertices_ == bfs.getBFSVertices() && Q_ == bfs.getQ() &&
            S_ == bfs.getS() && V_ == bfs.getV() && pathExists_ == bfs.pathExists());
}

bool BFS::operator!=(const BFS& bfs) const {
    return !(*this == bfs);
}

/* --------------------------------------------- GETTERS: -------------------------------------------- */

const vector<pair<int, int>>& BFS::getBFSPath() const {
    return BFSPath_;
}

const  vector<BFS_Vertex>& BFS::getBFSVertices() const {
    return BFSVertices_;
}

const  vector<BFS_Vertex>& BFS::getQ() const {
    return Q_;
}

BFS_Vertex BFS::getS() const {
    return S_;
}

BFS_Vertex BFS::getV() const {
    return V_;
}

bool BFS::pathExists() const {
    return pathExists_;
}

/* --------------------------------------------- SETTERS: -------------------------------------------- */

void BFS::setBFSPath(const vector<pair<int, int>>& bfs_path) {
    BFSPath_ = bfs_path;
}

void BFS::setBFSVertices(const vector<BFS_Vertex>& bfs_vertices) {
    BFSVertices_ = bfs_vertices;
}

void BFS::setQ(const vector<BFS_Vertex>& Q) {
    Q_ = Q;
}

void BFS::setS(BFS_Vertex S) {
    S_ = S;
}

void BFS::setV(BFS_Vertex V) {
    V_ = V;
}

void BFS::setPathExistance(bool path_exists) {
    pathExists_ = path_exists;
}

/* ----------------------------------------- OTHER FUNCTIONS: ---------------------------------------- */

void BFS::initializeBFSVertices(const std::vector<std::vector<char>>& grid) {
    BFSVertices_.clear();  // Clear any previous vertices
    int rows = grid.size();
    int cols = grid.empty() ? 0 : grid[0].size();

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            BFS_Vertex vertex({i, j});
            BFSVertices_.push_back(vertex);
        }
    }
}

void BFS::addBFSVertex(BFS_Vertex bfs_vertex) {
    BFSVertices_.push_back(bfs_vertex);
}

void BFS::updateBFSFields(pair<int, int> S_pos, pair<int, int> V_pos) {
    BFSPath_.clear();
    Q_.clear();
    for (BFS_Vertex bfs_vertex : BFSVertices_) {
        bfs_vertex.setHasParent(false);
        if (bfs_vertex.getVertexPos() == S_pos) {
            bfs_vertex.setColor("gray");
            bfs_vertex.setDis(0);
            S_ = bfs_vertex;
            Q_.push_back(S_);
        }
        else {
            bfs_vertex.setColor("white");
            bfs_vertex.setDis(numeric_limits<double>::infinity());
            if (bfs_vertex.getVertexPos() == V_pos) { V_ = bfs_vertex; }
        }
    }
    pathExists_ = false;
}

void BFS::reconstructBFSPath() {
    BFSPath_.clear();
    auto it = find_if(BFSVertices_.begin(), BFSVertices_.end(), [this](const BFS_Vertex& bfs_vertex)
        { return bfs_vertex.getVertexPos() == V_.getVertexPos(); });
    if (it == BFSVertices_.end() || !it->hasParent()) { /* No path exists. */
        pathExists_ = false;
        return;
    }
    BFS_Vertex current = *it;
    while (current.getVertexPos() != S_.getVertexPos()) {
        BFSPath_.push_back(current.getVertexPos());
        auto parent_it = find_if(BFSVertices_.begin(), BFSVertices_.end(),
            [&current](const BFS_Vertex& bfs_vertex)
            { return bfs_vertex.getVertexPos() == current.getParentPos(); });
        if (parent_it == BFSVertices_.end()) {  /* Path reconstruction failed. */
            pathExists_ = false;
            break;
        }
        current = *parent_it;
    }
    BFSPath_.push_back(S_.getVertexPos());
    reverse(BFSPath_.begin(), BFSPath_.end());
    pathExists_ = !BFSPath_.empty();  // Ensure pathExists_ is set correctly
}

void BFS::runBFS(pair<int, int> S_pos, pair<int, int> V_pos,
    const vector<vector<char>>& grid) {
    updateBFSFields(S_pos, V_pos);
    int rows = grid.size();
    int cols = grid[0].size();

    while (!Q_.empty()) {
        BFS_Vertex u = Q_.front();
        Q_.erase(Q_.begin());

        if (u.getVertexPos() == V_pos) {
            pathExists_ = true;
            reconstructBFSPath();
            break;
        }

        // Explore neighbors using 8 directions
        for (int dir_idx = 0; dir_idx < 8; dir_idx++) {
            Direction dir = indexToDirection(dir_idx);
            pair<int, int> neighbor = getNextCoordsInDir(dir, u.getVertexPos(), rows, cols);

            // Skip walls '#' and mines '@'
            if (grid[neighbor.first][neighbor.second] == '#' ||
                grid[neighbor.first][neighbor.second] == '@') {
                continue;
            }

            auto it = find_if(BFSVertices_.begin(), BFSVertices_.end(),
                [&neighbor](const BFS_Vertex& bfs_vertex)
                { return bfs_vertex.getVertexPos() == neighbor; });

            if (it != BFSVertices_.end() && it->getColor() == "white") {
                it->setColor("gray");
                it->setDis(u.getDis() + 1);
                it->setParentPos(u.getVertexPos());
                it->setHasParent(true);
                Q_.push_back(*it);
            }
        }

        auto it = find_if(BFSVertices_.begin(), BFSVertices_.end(),
            [&u](const BFS_Vertex& bfs_vertex)
            { return bfs_vertex.getVertexPos() == u.getVertexPos(); });
        if (it != BFSVertices_.end()) {
            it->setColor("black");
        }
    }
}

bool BFS::isOnPath(pair<int, int> pos) {
    return find(BFSPath_.begin(), BFSPath_.end(), pos) != BFSPath_.end();
}

void BFS::resetBFS() noexcept {
    BFSPath_.clear();
    vector<pair<int, int>>().swap(BFSPath_);
    BFSVertices_.clear();
    vector<BFS_Vertex>().swap(BFSVertices_);
    Q_.clear();
    vector<BFS_Vertex>().swap(Q_);
    S_.resetBFSVertex();
    V_.resetBFSVertex();
    pathExists_ = false;
}