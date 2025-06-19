/**
 * @file gamemanager.cpp
 * @brief A file detailing the implementation of all the functions declared in gamemanager.h.
 * The maximal number of columns in a comment-line in this file is 106.
 */

#include "gamemanager.h"
#include <sstream>
#include <fstream>
using namespace std;

/* ------------------------------------------ CONSTRUCTORS: ------------------------------------------ */

GameManager::GameManager() {
    board_ = Board();
    tankA_ = TankA();
    tankB_ = TankB();
    outputFileName_ = string("");
}

GameManager::GameManager(Board board, TankA tankA, TankB tankB, string output_file_name) {
    board_ = board;
    tankA_ = tankA;
    tankB_ = tankB;
    outputFileName_ = output_file_name;
}

GameManager::GameManager(const GameManager& game_manager) {
    board_ = game_manager.getBoard();
    tankA_ = game_manager.getTankA();
    tankB_ = game_manager.getTankB();
    outputFileName_ = game_manager.getOutputFileName();
}

GameManager::GameManager(GameManager&& game_manager) noexcept {
    board_ = game_manager.getBoard();
    tankA_ = game_manager.getTankA();
    tankB_ = game_manager.getTankB();
    outputFileName_ = game_manager.getOutputFileName();
    game_manager.resetGameManager();
}

/* -------------------------------------------- OPERATORS: ------------------------------------------- */

GameManager& GameManager::operator=(const GameManager& game_manager) {
    if (this != &game_manager) {
        board_ = game_manager.getBoard();
        tankA_ = game_manager.getTankA();
        tankB_ = game_manager.getTankB();
        outputFileName_ = game_manager.getOutputFileName();
    }
    return *this;
}

GameManager& GameManager::operator=(GameManager&& game_manager) noexcept {
    if (this != &game_manager) {
        board_ = game_manager.getBoard();
        tankA_ = game_manager.getTankA();
        tankB_ = game_manager.getTankB();
        outputFileName_ = game_manager.getOutputFileName();
        game_manager.resetGameManager();
    }
    return *this;
}

bool GameManager::operator==(const GameManager& game_manager) const {
    return (board_ == game_manager.getBoard() && tankA_ == game_manager.getTankA() && 
        tankB_ == game_manager.getTankB() &&  outputFileName_ == game_manager.getOutputFileName());
}

bool GameManager::operator!=(const GameManager& game_manager) const {
    return !(*this == game_manager);
}

/* --------------------------------------------- GETTERS: -------------------------------------------- */

Board GameManager::getBoard() const {
    return board_;
}

TankA GameManager::getTankA() const {
    return tankA_;
}

TankB GameManager::getTankB() const {
    return tankB_;
}

string GameManager::getOutputFileName() const {
    return outputFileName_;
}

/* --------------------------------------------- SETTERS: -------------------------------------------- */

void GameManager::setBoard(Board board) {
    board_ = board;
}

void GameManager::setTankA(TankA tankA) {
    tankA_ = tankA;
}

void GameManager::setTankB(TankB tankB) {
    tankB_ = tankB;
}

void GameManager::setOutputFileName(string output_file_name) {
    outputFileName_ = output_file_name;
}

/* ----------------------------------------- OTHER FUNCTIONS: ---------------------------------------- */

void GameManager::setupFromInput(const vector<string>& input_lines) {
    if (input_lines.empty()) { throw runtime_error("Input is empty"); }
    ofstream errorLog;
    bool hasErrors = false;
    auto logError = [&](const string& msg) {
        if (!hasErrors) {
            errorLog.open("input_errors.txt");
            hasErrors = true;
        }
        errorLog << msg << '\n';
    };

   // Read the board dimensions from the first line
    istringstream dimsStream(input_lines[0]);
    string token1, token2;
    dimsStream >> token1 >> token2;

    // Pad if missing
    if (token1.empty()) token1 = "0";
    if (token2.empty()) token2 = "0";

    size_t width, height;
    width = stoi(token1);
    height = stoi(token2);

    if (width <= 0 || height <= 0){ throw runtime_error("Invalid board dimentions"); }

    board_ = Board(height, width);
    bool addedTankA = false, addedTankB = false;

    for (size_t row = 0; row < height; ++row) {
        string line;
        if (row + 1 >= input_lines.size()) {
            // Missing row — fill with spaces
            line = string(width, ' ');
            logError("Missing row at index " + to_string(row));
        }
        else{
            line = input_lines[row + 1];
        }

        if (line.size() < width) {
            logError("Row " + to_string(row) + " shorter than declared width. Padding with spaces.");
            line.append(width - line.size(), ' ');
        } else if (line.size() > width) {
            logError("Row " + to_string(row) + " exceeds declared width. Extra characters will be ignored.");
            line.resize(width);
        }

        for (size_t col = 0; col < width; ++col) {
            char c = col < line.size() ? line[col] : ' ';
            pair<int, int> coord = {row, col};
            switch (c) {
                case ' ':
                    break;
                case '#':
                    board_.placeWall(coord);
                    break;
                case '@':
                    board_.placeMine(coord);
                    break;
                case '1':
                    if (!addedTankA) {
                        tankA_ = TankA(coord, Direction::L);
                        board_.placeTankA(coord);
                        addedTankA = true;
                    } else {
                        logError("Multiple tanks for player A. Ignoring tank at (" + to_string(col) + ", " + to_string(row) + ")");
                    }
                    break;
                case '2':
                    if (!addedTankB) {
                        tankB_ = TankB(coord, Direction::R);
                        board_.placeTankB(coord);
                        addedTankB = true;
                    } else {
                        logError("Multiple tanks for player B. Ignoring tank at (" + to_string(col) + ", " + to_string(row) + ")");
                    }
                    break;
                default:
                    logError("Unknown character '" + string(1, c) + "' at (" + to_string(col) + ", " + to_string(row) + "). Treated as space.");
                    break;
            }
        }
    }

    if (!addedTankA || !addedTankB) {
        throw runtime_error("Both tanks must be present on the board.");
    }

    cout << "Board setup complete:\n";
    board_.render();

    if (hasErrors) { errorLog.close(); }

    // Fill vertices based on grid
    for (int row = 0; row < static_cast<int>(height); row++) {
        for (int col = 0; col < static_cast<int>(width); col++) {
            std::string cell = board_.getGrid()[row][col];
            if (cell == "#") {
                board_.addNewVertexToMap(row, col, true, false);
            } else if (cell == "@") {
                board_.addNewVertexToMap(row, col, false, true);
            } else {
                board_.addNewVertexToMap(row, col);
            }
        }
    }

    // Create BFS properly
    BFS bfs(board_.getRows() * board_.getColumns());
    for (const auto& pair : board_.getVertices()) {
        bfs.addBFSVertex(BFS_Vertex(pair.first));
    }
    pair<int, int> source = tankA_.getPos();
    pair<int, int> destination = tankB_.getPos();
    bfs.runBFS(source, destination, board_.getVertices());
    board_.setBFS(bfs);
}

void GameManager::updateObjectsOnBoard() {
    vector<Shell> newShells;
    for (auto& shell : board_.getShells()) {
        Shell shellPos = shell;
        auto [x, y] = shellPos.getPos();
        // check collision with tanks
        if (tankA_.getPos() == shellPos.getPos()) {
            tankA_.kaboom();
            shell.setExploded(true);
        }

        if (tankB_.getPos() == shellPos.getPos()) {
            tankB_.kaboom();
            shell.setExploded(true);
        }

        // now check for collision with walls
        Vertex shell_vertex;
        for (auto& pair : board_.getVertices()) {
            if (pair.first == shellPos.getPos()) {
                shell_vertex = pair.second;
                break;
            }
        }
        if (shell_vertex.isWall()){
            shell_vertex.increaseHP();
            shell.setExploded(true);
            if (shell_vertex.shouldWreckWall()){
                shell_vertex.wreckWall();
                board_.getGrid()[x][y] = ".";
            }
        }

        // only add shells that didn't explode this round
        if(!shell.hasExploded()) { newShells.push_back(shell); }
        else {
            if  (board_.getGrid()[x][y] == "*@") {
                board_.getGrid()[x][y] = "@";
            }
            else {
                board_.getGrid()[x][y] = ".";
            }
        }
     }

    // kill tanks if they step on mines
    Vertex tankA_vertex, tankB_vertex;
    for (auto& pair : board_.getVertices()) {
        if (pair.first == tankA_.getPos()) { tankA_vertex = pair.second; }
        if (pair.first == tankB_.getPos()) { tankB_vertex = pair.second; }
    }
    if (tankA_vertex.isMine()) { tankA_.stepOnAMine(); }

    if (tankB_vertex.isMine()) { tankB_.stepOnAMine(); }

    // kill tanks if they hit each other
    if (tankA_.getPos() == tankB_.getPos()) {
        tankA_.batteringRam();
        tankB_.batteringRam();
    }

    auto [x1, y1] = tankA_.getPos();
    auto [x2, y2] = tankB_.getPos();
    board_.getGrid()[x1][y1] = "1";
    board_.getGrid()[x2][y2] = "2";
    board_.setPosTankA(tankA_.getPos());

    cout << "Tank A is now at position (" <<
        board_.getPosTankA().first << ", " <<
        board_.getPosTankA().second << ")." << endl;

    board_.setPosTankB(tankB_.getPos());

    cout << "Tank B is now at position (" <<
        board_.getPosTankB().first << ", " <<
        board_.getPosTankB().second << ")." << endl;

    board_.setShells(newShells);
}

void GameManager::startGame() {
    int rounds = 0;

    // by the advice given in the forum, we first move all the shells, check for hits,
    // move the tanks, and then check for hits again
    while (tankA_.getAmmo() > 0 || tankB_.getAmmo() > 0)
    {
        cout << "===Round " << rounds << "===" << endl;
        board_.render();
        board_.moveAllShells();
        updateObjectsOnBoard();
        if (gameOver()){
            return;
        }
        if (rounds % 2 == 0){
            Action action1 = tankA_.nextAction(board_);
            Action action2 = tankB_.nextAction(board_);
            board_.removeTankFromGrid(tankA_.getPos());
            board_.removeTankFromGrid(tankB_.getPos());
            bool action1Success = tankA_.act(action1, board_);
            if (action1Success) {
                writeToOutputLog("tankA action: " + actionToString(action1));
                cout << "tankA action: " << actionToString(action1) << endl;
                if (action1 == Action::Shoot) {
                    pair<int, int> pos = tankA_.getPos();
                    Direction dir = tankA_.getDir();
                    pair<int, int> nextPos = getNextCoordsInDir(dir, pos, board_.getRows(), board_.getColumns());
                    board_.addNewShell(nextPos.first, nextPos.second, dir);
                }
            }
            else { 
                writeToOutputLog("tankA action: bad step");
                cout << "tankA bad action: " << actionToString(action1) << endl;
            }
            bool action2Success = tankB_.act(action2, board_);
            if (action2Success) {
                writeToOutputLog("tankB action: " + actionToString(action2));
                cout << "tankB action: " << actionToString(action2) << endl;
                if (action2 == Action::Shoot) {
                    pair<int, int> pos = tankB_.getPos();
                    Direction dir = tankB_.getDir();
                    pair<int, int> nextPos = getNextCoordsInDir(dir, pos, board_.getRows(), board_.getColumns());
                    board_.addNewShell(nextPos.first, nextPos.second, dir);
                }
            }
            else { 
                writeToOutputLog("tankB action: bad step");
                cout << "tankB bad action: " << actionToString(action2) << endl;
            }
            updateObjectsOnBoard();
        }
        if (gameOver()) { return; }
        rounds += 1;
    }

    for (size_t i = 0; i < 80; i++) // 80 because its 40 game plays for the tanks
    {
        board_.moveAllShells();
        updateObjectsOnBoard();
        if (gameOver()) { return; }
        if (i % 2 == 0){
            Action action1 = tankA_.nextAction(board_);
            Action action2 = tankB_.nextAction(board_);
            board_.removeTankFromGrid(tankA_.getPos());
            board_.removeTankFromGrid(tankB_.getPos());
            bool action1Success = tankA_.act(action1, board_);
            if (action1Success) {
                writeToOutputLog("tankA action: " + actionToString(action1));
                if (action1 == Action::Shoot) {
                    pair<int, int> pos = tankA_.getPos();
                    Direction dir = tankA_.getDir();
                    pair<int, int> nextPos = getNextCoordsInDir(dir, pos, board_.getRows(), board_.getColumns());
                    board_.addNewShell(nextPos.first, nextPos.second, dir);
                }
            }
            else { writeToOutputLog("tankA action: bad step"); }
            bool action2Success = tankB_.act(action2, board_);
            if (action2Success) {
                writeToOutputLog("tankB action: " + actionToString(action2));
                if (action2 == Action::Shoot) {
                    pair<int, int> pos = tankB_.getPos();
                    Direction dir = tankB_.getDir();
                    pair<int, int> nextPos = getNextCoordsInDir(dir, pos, board_.getRows(), board_.getColumns());
                    board_.addNewShell(nextPos.first, nextPos.second, dir);
                }
            }
            else { writeToOutputLog("tankB action: bad step"); }
            updateObjectsOnBoard();
        }
        if (gameOver()) { return; }
    }
    
    writeToOutputLog("Game Over! its a tie! (both players are still alive)");
}

bool GameManager::gameOver() {
    if (!tankA_.alive() && !tankB_.alive()) {
        writeToOutputLog("Game Over! its a tie!");
        writeToOutputLog( "tankA " + tankA_.getCauseOfDeath() + " tankB " + tankB_.getCauseOfDeath());
        return true;
    } else if(!tankA_.alive()){
        writeToOutputLog("Game Over! tank B won!");
        writeToOutputLog( "tankA " + tankA_.getCauseOfDeath());
        return true;
    } else if(!tankB_.alive()){
        writeToOutputLog("Game Over! tank A won!");
        writeToOutputLog( "tankB " + tankB_.getCauseOfDeath());
        return true;
    }

    return false;
}

void GameManager::writeToErrorLog(const string& message) {
    const string input_errors_file = "input_errors.txt";
 
     ofstream errorLog(input_errors_file, ios::app);
 
     if (errorLog.is_open()) {
         errorLog << message << '\n';
         errorLog.close();
     }
}

void GameManager::writeToOutputLog(const string& message) {
    ofstream outputLog(outputFileName_, ios::app);

    if (outputLog.is_open()) {
        outputLog << message << '\n';
        outputLog.close();
    }
}

void GameManager::resetGameManager() {
    board_.resetBoard();
    tankA_.resetTank();
    tankB_.resetTank();
    outputFileName_.clear();
    string().swap(outputFileName_);
}