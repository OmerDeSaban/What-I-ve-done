/**
 * @file board.cpp
 * @brief A file detailing the implementation of all the functions declared in board.h.
 * The maximal number of columns in a comment-line in this file is 106.
 */

#include "board.h"
#include <algorithm>
using namespace std;

/* ------------------------------------------ CONSTRUCTORS: ------------------------------------------ */

Board::Board() {
    rows_ = 0;
    columns_ = 0;
    grid_ = vector<vector<string>>();
    vertices_ = map<pair<int, int>, Vertex>();
    shells_ = vector<Shell>();
    bfs_ = BFS();
    posTankA_ = {0, 0};
    posTankB_ = {0, 0};
}

Board::Board(int rows, int columns) {
    rows_ = rows;
    columns_ = columns;

    grid_.resize(rows_);
    for (int i = 0; i < rows_; ++i) {
        grid_[i].resize(columns_, ".");
    }

    vertices_ = map<pair<int, int>, Vertex>();
    shells_ = vector<Shell>();
    shells_.reserve(32);
    bfs_ = BFS(rows * columns);
    posTankA_ = {0, 0};
    posTankB_ = {0, 0};
}

Board::Board(int rows, int columns, const vector<vector<string>>& grid,
    const map<pair<int, int>, Vertex>& vertices, const vector<Shell>& shells, BFS bfs,
    pair<int, int> tankA_pos, pair<int, int> tankB_pos) {
        rows_ = rows;
        columns_ = columns;
        grid_ = grid;
        vertices_ = vertices;
        shells_ = shells;
        bfs_ = bfs;
        posTankA_ = tankA_pos;
        posTankB_ = tankB_pos;
    }

Board::Board(const Board& board) {
    rows_ = board.getRows();
    columns_ = board.getColumns();
    grid_ = board.getGrid();
    vertices_ = board.getVertices();
    shells_ = board.getShells();
    bfs_ = board.getBFS();
    posTankA_ = board.getPosTankA();
    posTankB_ = board.getPosTankB();
}

Board::Board(Board&& board) noexcept {
    rows_ = board.getRows();
    columns_ = board.getColumns();
    grid_ = board.getGrid();
    vertices_ = board.getVertices();
    shells_ = board.getShells();
    bfs_ = board.getBFS();
    posTankA_ = board.getPosTankA();
    posTankB_ = board.getPosTankB();
    board.resetBoard();
}

/* -------------------------------------------- OPERATORS: ------------------------------------------- */

Board& Board::operator=(const Board& board) {
    if (this != &board) {
        rows_ = board.getRows();
        columns_ = board.getColumns();
        grid_ = board.getGrid();
        vertices_ = board.getVertices();
        shells_ = board.getShells();
        bfs_ = board.getBFS();
        posTankA_ = board.getPosTankA();
        posTankB_ = board.getPosTankB();
    }
    return *this;
}

Board& Board::operator=(Board&& board) noexcept {
    if (this != &board) {
        rows_ = board.getRows();
        columns_ = board.getColumns();
        grid_ = board.getGrid();
        vertices_ = board.getVertices();
        shells_ = board.getShells();
        bfs_ = board.getBFS();
        posTankA_ = board.getPosTankA();
        posTankB_ = board.getPosTankB();
        board.resetBoard();
    }
    return *this;
}

bool Board::operator==(const Board& board) const {
    return (rows_ == board.getRows() && columns_ == board.getColumns() && grid_ == board.getGrid() &&
        vertices_ == board.getVertices() &&  shells_ == board.getShells() && bfs_ == board.getBFS() &&
        posTankA_ == board.getPosTankA() &&  posTankB_ == board.getPosTankB());
}

bool Board::operator!=(const Board& board) const {
    return !(*this == board);
}

/* --------------------------------------------- GETTERS: -------------------------------------------- */

int Board::getRows() const {
    return rows_;
}

int Board::getColumns() const {
    return columns_;
}

const vector<vector<string>>& Board::getGrid() const {
    return grid_;
}

vector<vector<string>>& Board::getGrid() {
    return grid_;
}

const map<pair<int, int>, Vertex>& Board::getVertices() const {
    return vertices_;
}

const vector<Shell>& Board::getShells() const {
    return shells_;
}

vector<Shell>& Board::getShells() {
    return shells_;
}

BFS Board::getBFS() const {
    return bfs_;
}

pair<int, int> Board::getPosTankA() const {
    return posTankA_;
}

pair<int, int> Board::getPosTankB() const {
    return posTankB_;
}

/* --------------------------------------------- SETTERS: -------------------------------------------- */

void Board::setRows(int rows) {
    rows_ = rows;
}

void Board::setColumns(int columns) {
    columns_ = columns;
}

void Board::setGrid(const vector<vector<string>>& grid) {
    grid_ = grid;
}

void Board::setVertices(const map<pair<int, int>, Vertex>& vertices) {
    vertices_ = vertices;
}

void Board::setShells(const vector<Shell>& shells) {
    shells_ = shells;
}

void Board::setBFS(BFS bfs) {
    bfs_ = bfs;
}

void Board::setPosTankA(const pair<int, int> tankA_pos) {
    posTankA_ = tankA_pos;
}

void Board::setPosTankB(const pair<int, int> tankB_pos) {
    posTankB_ = tankB_pos;
}

/* ------------------------------------- OTHER FUNCTIONS: ------------------------------------ */

void Board::render() const {
    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < columns_; j++) {
            cout << grid_[i][j] << " ";
        }
        cout << endl;
    }
}

int Board::getNeighborXCoord(int currentX, int deltaX) {
    int newX = currentX + deltaX;
    if (newX < 0) { newX = rows_ - 1; }
    else if (newX >= rows_) { newX = 0; }
    return newX;
}

int Board::getNeighborYCoord(int currentY, int deltaY) {
    int newY = currentY + deltaY;
    if (newY < 0) { newY = columns_ - 1; }
    else if (newY >= columns_) { newY = 0; }
    return newY;
}

pair<int, int> Board::getNeighborCoords(pair<int, int> current_pos, Direction dir) {
    auto [deltaX, deltaY] = directionToDelta(dir);
    return {getNeighborXCoord(current_pos.first, deltaX), getNeighborYCoord(current_pos.second, deltaY)};
}

void Board::placeTankA(pair<int, int> pos) {
    grid_[pos.first][pos.second] = "1";
    posTankA_ = pos;
}

void Board::placeTankB(pair<int, int> pos) {
    grid_[pos.first][pos.second] = "2";
    posTankB_ = pos;
}

void Board::placeWall(pair<int, int> pos) {
    grid_[pos.first][pos.second] = "#";
    for (auto pair : vertices_) {
        if (pair.first == pos) {
            pair.second.setHP(0);
            pair.second.setWall(true);
        }
    }
}

void Board::placeMine(pair<int, int> pos) {
    grid_[pos.first][pos.second] = "@";
    for (auto pair : vertices_) { if (pair.first == pos) { pair.second.setMine(true); } }
}

void Board::addNewVertexToMap(int X, int Y, bool is_wall, bool is_mine) {
    vector<pair<int, int>> adj = vector<pair<int, int>>();
    adj.reserve(8);
    for (int i = 0; i < 8; i++) {
        Direction dir = indexToDirection(i);
        adj.push_back(getNeighborCoords({X, Y}, dir));
    }
    Vertex vertex(adj, X, Y, 0, is_wall, is_mine);
    if (is_wall) {
        vertex.setHP(0);
        grid_[X][Y] = "#";
    }
    else if (is_mine) { grid_[X][Y] = "@"; }
    vertices_[{X, Y}] = vertex;
}

void Board::addExistingVertexToMap(Vertex vertex) {
    if (vertex.isWall()) { grid_[vertex.getX()][vertex.getY()] = "#"; }
    if (vertex.isMine()) { grid_[vertex.getX()][vertex.getY()] = "@"; }
    vertices_[{vertex.getX(), vertex.getY()}] = vertex;
}

void Board::initializeBoard() {
    /* In this implementation, the grid would be fully initialized by the game manager! */
    // Initializing vertices, map, and BFS vertices:
    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < columns_; j++) {
            if (grid_[i][j] == "#") { addNewVertexToMap(i, j, true, false); }
            else if (grid_[i][j] == "@") { addNewVertexToMap(i, j, false, true); }
            else { addNewVertexToMap(i, j); }
            BFS_Vertex bfs_vertex({i, j});
            bfs_.addBFSVertex(bfs_vertex);
        }
    }
}

void Board::addNewShell(int X, int Y, Direction dir) {
    vector<pair<int, int>> affected_positions;
    affected_positions.reserve(8);
    affected_positions.push_back({X, Y});
    pair<int, int> pos = {X, Y};
    for (int i = 0; i < 7; i++) {
        pos = getNeighborCoords(pos, dir);
        affected_positions.push_back(pos);
    }
    Shell shell(affected_positions, {X, Y}, dir, false);
    shells_.push_back(shell);
    if (grid_[X][Y] == "@") { grid_[X][Y] = "*@"; }
    else { grid_[X][Y] = "*"; }
}

void Board::removeShell(const Shell& shell) {
    shells_.erase(remove(shells_.begin(), shells_.end(), shell), shells_.end());
    grid_[shell.getPos().first][shell.getPos().second] = ".";
}

bool Board::isBFSPathSafe() {
    for (const pair<int, int>& vertex_pos : bfs_.getBFSPath()) {
        if (vertex_pos == posTankA_ || vertex_pos == posTankB_) { continue; }
        else if (!isSafe(vertex_pos)) { return false; }
    }
    return true;
}

void Board::moveShell(Shell& shell) {
    if (shell.getAffectedPositions().empty()) { throw out_of_range("Shell has no more moves!"); }
    pair<int, int> next_pos = shell.getAffectedPositions().front();
    shell.getAffectedPositions().erase(shell.getAffectedPositions().begin());
    if (grid_[shell.getPos().first][shell.getPos().second] == "*@") {
        grid_[shell.getPos().first][shell.getPos().second] = "@";
    }
    else { grid_[shell.getPos().first][shell.getPos().second] = "."; }
    shell.setPos(next_pos);
    if (grid_[shell.getPos().first][shell.getPos().second] == "@") {
        grid_[shell.getPos().first][shell.getPos().second] = "*@";
    }
    else { grid_[shell.getPos().first][shell.getPos().second] = "*"; }
    shell.addAffectedPosition(getNeighborCoords(shell.getAffectedPositions().back(), shell.getDir()));
}

void Board::moveAllShells() {
    // before moving the shells, check for their intersection
    int n = shells_.size();
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            pair<int, int> shell1Pos = shells_[i].getPos();
            Direction shell1Dir = shells_[i].getDir();
            pair<int, int> shell1NextPos = getNextCoordsInDir(shell1Dir, shell1Pos, getRows(), getColumns());

            pair<int, int> shell2Pos = shells_[j].getPos();
            Direction shell2Dir = shells_[j].getDir();
            pair<int, int> shell2NextPos = getNextCoordsInDir(shell2Dir, shell2Pos, getRows(), getColumns());

            if (intersect(shell1Pos, shell1NextPos, shell2Pos, shell2NextPos)){
                shells_[i].setExploded(true);
                shells_[j].setExploded(true);
            }
    
        }
    }
    for (Shell& shell : shells_) { moveShell(shell); }
}

bool Board::placeTaken(pair<int, int> pos) {
    return (grid_[pos.first][pos.second] != "." && grid_[pos.first][pos.second] != "1" && grid_[pos.first][pos.second] != "2");
}

bool Board::isSafe(pair<int, int> pos) {
    // Not a wall, a mine, or a shell
    if (placeTaken(pos)) { return false; }

    // Shell prediction (next 4 tank turns)
    for (const auto& shell : shells_) {
        pair<int, int> tmp = shell.getPos();
        Direction dir = shell.getDir();

        for (int step = 0; step < 4; ++step) {
            tmp = getNeighborCoords(tmp, dir);
            if (pos.first == tmp.first && pos.second == tmp.second) { return false; }
        }
    }

    return true;
}

bool Board::intersect(pair<int, int> objectA_start, pair<int, int> objectA_des,
    pair<int, int> objectB_start, pair<int, int> objectB_des) {
        int objectA_Xmid = (objectA_start.first + objectA_des.first)/2;
        int objectA_Ymid = (objectA_start.second + objectA_des.second)/2;
        int objectB_Xmid = (objectB_start.first + objectB_des.first)/2;
        int objectB_Ymid = (objectB_start.second + objectB_des.second)/2;
        // Checking if both object are destined to reach the same position, where they would collide:
        bool sameDestination = (objectA_des == objectB_des);
        // Checking if the objects would intersect at a diagonal, or halfway through their movement:
        bool diagonalIntersection = (objectA_Xmid == objectB_Xmid && objectA_Ymid == objectB_Ymid);
        return (sameDestination || diagonalIntersection);
    }

void Board::removeTankFromGrid(pair<int, int> pos) {
    grid_[pos.first][pos.second] = ".";
}

void Board::resetBoard() {
    rows_ = 0;
    columns_ = 0;
    grid_.clear();
    vector<vector<string>>().swap(grid_);
    vertices_.clear();
    map<pair<int, int>, Vertex>().swap(vertices_);
    shells_.clear();
    vector<Shell>().swap(shells_);
    bfs_.resetBFS();
    posTankA_ = pair<int, int>();
    posTankB_ = pair<int, int>();
}