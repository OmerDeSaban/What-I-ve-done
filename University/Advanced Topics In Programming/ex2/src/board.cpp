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
    tanks_ = vector<Tank>();
    playerATanks_ = 0;
    playerBTanks_ = 0;
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
    tanks_ = vector<Tank>();
    playerATanks_ = 0;
    playerBTanks_ = 0;
}

Board::Board(int rows, int columns, const vector<vector<string>>& grid,
    const map<pair<int, int>, Vertex>& vertices, const vector<Shell>& shells ) {
        rows_ = rows;
        columns_ = columns;
        grid_ = grid;
        vertices_ = vertices;
        shells_ = shells;
    }

Board::Board(const Board& board) {
    rows_ = board.getRows();
    columns_ = board.getColumns();
    grid_ = board.getGrid();
    vertices_ = board.getVertices();
    shells_ = board.getShells();
    // bfs_ = board.getBFS();
}

Board::Board(Board&& board) noexcept {
    rows_ = board.getRows();
    columns_ = board.getColumns();
    grid_ = board.getGrid();
    vertices_ = board.getVertices();
    shells_ = board.getShells();
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
        board.resetBoard();
    }
    return *this;
}

bool Board::operator==(const Board& board) const {
    return (rows_ == board.getRows() && columns_ == board.getColumns() && grid_ == board.getGrid() &&
        vertices_ == board.getVertices() &&  shells_ == board.getShells());
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

int Board::getPlayerATanks() const{
    return playerATanks_;
}

int Board::getPlayerBTanks() const{
    return playerBTanks_;
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

void Board::placeTankA(pair<int, int> pos, int ammo) {
    grid_[pos.first][pos.second] = "1";
    Tank newTank = Tank(pos, Direction::L, 1, ammo);
    tanks_.push_back(newTank);
    playerATanks_++;
}

void Board::placeTankB(pair<int, int> pos, int ammo) {
    grid_[pos.first][pos.second] = "2";
    Tank newTank = Tank(pos, Direction::R, 2, ammo);
    tanks_.push_back(newTank);
    playerBTanks_++;
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
    // Initializing vertices
    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < columns_; j++) {
            if (grid_[i][j] == "#") { addNewVertexToMap(i, j, true, false); }
            else if (grid_[i][j] == "@") { addNewVertexToMap(i, j, false, true); }
            else { addNewVertexToMap(i, j); }
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

bool Board::move(ActionRequest action, int tankInd) {
    std::pair<int, int> next_pos;
    std::pair<int, int> current_pos = tanks_[tankInd].getPos();
    Direction dir = tanks_[tankInd].getDir();
    removeTankFromGrid(current_pos);

    if (action == ActionRequest::MoveForward) {
        if (tanks_[tankInd].getBackCnt() > 0) {
            tanks_[tankInd].resetBackCnt();
            return true;
        }
        next_pos = this->getVertices().at(current_pos).getAdjCoordInDirection(dir);
        if (placeTaken(next_pos)){
            return false;
        }
        tanks_[tankInd].setPos(next_pos);
    } else if (action == ActionRequest::MoveBackward) {
        if (tanks_[tankInd].getBackCnt() == -1) {
            tanks_[tankInd].setBackCnt(2);
        } else if (tanks_[tankInd].getBackCnt() == 0) {
            Direction opposite = rotateRightQuarter(rotateRightQuarter(dir));
            next_pos = this->getVertices().at(current_pos).getAdjCoordInDirection(opposite);
            if (placeTaken(next_pos)){
                return false;
            }
            tanks_[tankInd].setPos(next_pos);
            tanks_[tankInd].setBackCnt(0);  // Enable chaining
        }
    }
    return true;
}

bool Board::shoot(int tankInd) {
    if (tanks_[tankInd].getAmmo() <= 0 || tanks_[tankInd].getLastShotCnt() > 0) {
        return false;
    }
    tanks_[tankInd].decreaseAmmo();
    tanks_[tankInd].setLastShotCnt(4);

    pair<int, int> pos = tanks_[tankInd].getPos();
    Direction dir = tanks_[tankInd].getDir();
    pair<int, int> nextPos = getNextCoordsInDir(dir, pos, rows_, columns_);
    addNewShell(nextPos.first, nextPos.second, dir);
    return true;
}

bool Board::rotate(ActionRequest action, int tankInd) {
    Direction dir = tanks_[tankInd].getDir();
    Direction newDirection = dir;

    if (action == ActionRequest::RotateLeft90) {
        newDirection = rotateLeftQuarter(dir);
    } else if (action == ActionRequest::RotateRight90) {
        newDirection = rotateRightQuarter(dir);
    } else if (action == ActionRequest::RotateLeft45) {
        newDirection = rotateLeftEighth(dir);
    } else if (action == ActionRequest::RotateRight45) {
        newDirection = rotateRightEighth(dir);
    }

    tanks_[tankInd].setDir(newDirection);
    return true;
}

bool Board::act(ActionRequest action, int tankInd) {
    tanks_[tankInd].decreaseLastShotCount();

    if (tanks_[tankInd].shouldWait(action)) {
        tanks_[tankInd].decreaseBackCnt();
        return false;
    }

    if (action == ActionRequest::MoveForward || action == ActionRequest::MoveBackward) {
        return move(action, tankInd);
    } else if (action == ActionRequest::RotateLeft45 || action == ActionRequest::RotateLeft90 ||
               action == ActionRequest::RotateRight45 || action == ActionRequest::RotateRight90) {
        return rotate(action, tankInd);
    } else if (action == ActionRequest::Shoot) {
        return shoot(tankInd);
    } else if (action == ActionRequest::DoNothing) {
        return true;
    }

    return false;
}

std::pair<int, int> Board::getPosTank(int i) const{
    return tanks_[i].getPos();
}

vector<pair<size_t, size_t>> Board::updateObjectsOnBoard() {
    vector<Shell> newShells;

    // Update all shells
    for (auto& shell : getShells()) {
        Shell shellPos = shell;
        auto [x, y] = shellPos.getPos();

        // Check collision with any tank
        for (auto& tank : tanks_) {
            if (!tank.alive()){
                continue;
            }
            if (tank.getPos() == shellPos.getPos()) {
                tank.kaboom();
                if (tank.getPlayerInd() == 1){
                    playerATanks_--;
                }
                else{
                    playerBTanks_--;
                }
                removeTankFromGrid(tank.getPos());
                shell.setExploded(true);
                break;
            }
        }

        // Now check for collision with walls
        Vertex shell_vertex;
        for (auto& pair : getVertices()) {
            if (pair.first == shellPos.getPos()) {
                shell_vertex = pair.second;
                break;
            }
        }

        if (shell_vertex.isWall()) {
            shell_vertex.increaseHP();
            shell.setExploded(true);
            if (shell_vertex.shouldWreckWall()) {
                shell_vertex.wreckWall();
                grid_[x][y] = ".";
            }
        }

        // Only add shells that didn't explode
        if (!shell.hasExploded()) {
            newShells.push_back(shell);
        } else {
            if (grid_[x][y] == "*@") {
                grid_[x][y] = "@";
            } else {
                grid_[x][y] = ".";
            }
        }
    }

    // Kill tanks if they step on mines
    for (auto& pair : getVertices()) {
        for (auto& tank : tanks_) {
            if (!tank.alive()){
                continue;
            }
            if (pair.first == tank.getPos()) {
                if (pair.second.isMine()) {
                    tank.stepOnAMine();
                    if (tank.getPlayerInd() == 1){
                        playerATanks_--;
                    }
                    else{
                        playerBTanks_--;
                    }
                    removeTankFromGrid(tank.getPos());
                }
            }
        }
    }

    // Kill tanks if they hit each other
    for (size_t i = 0; i < tanks_.size(); ++i) {
        if (!tanks_[i].alive()){
            continue;
        }
        for (size_t j = i + 1; j < tanks_.size(); ++j) {
            if (!tanks_[j].alive()){
                continue;
            }
            if (tanks_[i].getPos() == tanks_[j].getPos()) {
                tanks_[i].batteringRam();
                tanks_[j].batteringRam();
                if (tanks_[i].getPlayerInd() == 1){
                    playerATanks_--;
                }
                else{
                    playerBTanks_--;
                }
                if (tanks_[j].getPlayerInd() == 1){
                    playerATanks_--;
                }
                else{
                    playerBTanks_--;
                }
                removeTankFromGrid(tanks_[i].getPos());
                removeTankFromGrid(tanks_[j].getPos());
            }
        }
    }

    // Update tank positions on the board
    for (size_t i = 0; i < tanks_.size(); ++i) {
        if (!tanks_[i].alive()){
            continue;
        }
        auto [x, y] = tanks_[i].getPos();
        grid_[x][y] = std::to_string(tanks_[i].getPlayerInd());
    }

    // Update shells on the board
    setShells(newShells);

    vector<pair<size_t, size_t>> newTankPositions;
    // Update the positions of tanks in the UI (or other purposes)
    for (size_t i = 0; i < tanks_.size(); ++i) {
        auto pos = getPosTank(i);
        Tank tank = tanks_[i];
        if (!tank.alive()){
            continue;
        }
        if (tank.getPlayerInd() == 1){
            grid_[pos.first][pos.second] = "1";
        }
        else{
            grid_[pos.first][pos.second] = "2";
        }
        newTankPositions.emplace_back(static_cast<size_t>(pos.first), static_cast<size_t>(pos.second));
        cout << "Tank " << i + 1 << " is now at position (" <<
            getPosTank(i).first << ", " <<
            getPosTank(i).second << ")." << endl;
    }
    return newTankPositions;
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
}

bool Board::isTankAlive(int tankInd){
    return tanks_[tankInd].alive();
}


bool Board::tankHasAmmo(int tankInd) {
    return tanks_[tankInd].getAmmo() > 0;
}