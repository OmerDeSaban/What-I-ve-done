/**
 * @file gamemanager.cpp
 * @brief A file detailing the implementation of all the functions declared in gamemanager.h.
 * The maximal number of columns in a comment-line in this file is 106.
 */

#include "gamemanager.h"
#include <sstream>
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;
using namespace std;

/* ------------------------------------------ CONSTRUCTORS: ------------------------------------------ */
GameManager::GameManager(std::unique_ptr<PlayerFactory> playerFactory, std::unique_ptr<TankAlgorithmFactory> tankAlgorithmFactory)
    : board_(),
      playerFactory_(std::move(playerFactory)),
      tankAlgorithmFactory_(std::move(tankAlgorithmFactory)),
      player1_(nullptr),
      player2_(nullptr),
      gameOver_(false),
      maxRounds_(0),
      ammoLeft_(false),
      roundsWithoutAmmo_(0) {
    // Initialize satelliteView as a BaseSatelliteView instance
    satelliteView_ = std::make_unique<BaseSatelliteView>();
    tankPlayerIndices_ = std::vector<int>();
}

/* -------------------------------------------- OPERATORS: ------------------------------------------- */

GameManager& GameManager::operator=(const GameManager& game_manager) {
    if (this != &game_manager) {
        board_ = game_manager.getBoard();
        outputFileName_ = game_manager.getOutputFileName();
    }
    return *this;
}

GameManager& GameManager::operator=(GameManager&& game_manager) noexcept {
    if (this != &game_manager) {
        board_ = game_manager.getBoard();
        outputFileName_ = game_manager.getOutputFileName();
        game_manager.resetGameManager();
    }
    return *this;
}

bool GameManager::operator==(const GameManager& game_manager) const {
    return (board_ == game_manager.getBoard() &&  outputFileName_ == game_manager.getOutputFileName());
}

bool GameManager::operator!=(const GameManager& game_manager) const {
    return !(*this == game_manager);
}

/* --------------------------------------------- GETTERS: -------------------------------------------- */

Board GameManager::getBoard() const {
    return board_;
}

string GameManager::getOutputFileName() const {
    return outputFileName_;
}

/* --------------------------------------------- SETTERS: -------------------------------------------- */

void GameManager::setBoard(Board board) {
    board_ = board;
}

void GameManager::setOutputFileName(string output_file_name) {
    outputFileName_ = output_file_name;
}

/* ----------------------------------------- OTHER FUNCTIONS: ---------------------------------------- */

void GameManager::readBoard(std::string input_file_name) {
    vector<string> inputLines = readInputFile(input_file_name);
    outputFileName_ = getOutputFilename(input_file_name);
    if (inputLines.empty()) { throw runtime_error("Input is empty"); }
    ofstream errorLog;
    bool hasErrors = false;
    auto logError = [&](const string& msg) {
        if (!hasErrors) {
            errorLog.open("input_errors.txt");
            hasErrors = true;
        }
        errorLog << msg << '\n';
    };

    if (inputLines.size() < 6)
        throw std::runtime_error("Input file is too short");

    std::string maxStepsLine = inputLines[1];
    std::string numShellsLine = inputLines[2];
    std::string rowsLine = inputLines[3];
    std::string colsLine = inputLines[4];

    auto getNumFromLine = [](const std::string& line) {
        size_t eq = line.find('=');
        if (eq == std::string::npos)
            throw std::runtime_error("Line missing '=': " + line);
        return std::stoi(line.substr(eq + 1));
    };

    maxRounds_ = getNumFromLine(maxStepsLine);
    int numShells = getNumFromLine(numShellsLine);
    if (numShells > 0) { ammoLeft_ = true; }
    size_t height = getNumFromLine(rowsLine);
    size_t width = getNumFromLine(colsLine);
    cout << "===max rounds: " << maxRounds_ << " shells: " << numShells << " height: " << height << " width " << width << "===" << endl;

    if (width <= 0 || height <= 0){ throw runtime_error("Invalid board dimentions"); }

    board_ = Board(height, width);
    satelliteView_->initializeGrid(height, width);
    player1_ = playerFactory_->create(1, height, width, maxRounds_, numShells);
    player2_ = playerFactory_->create(2, height, width, maxRounds_, numShells);
    int player1Tanks, player2Tanks = 0;

    for (size_t row = 0; row < height; ++row) {
        string line;
        if (row + 5 >= inputLines.size()) {
            // Missing row — fill with spaces
            line = string(width, ' ');
            logError("Missing row at index " + to_string(row));
        }
        else{
            line = inputLines[row + 5];
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
                {
                    auto tankA = tankAlgorithmFactory_->create(1, tanksList_.size());
                    tanksList_.push_back(std::move(tankA));
                    board_.placeTankA(coord, numShells);
                    satelliteView_->addPosition(row, col);
                    player1Tanks += 1;
                    tankPlayerIndices_.push_back(1);
                    break;
                }
                case '2':
                {
                    auto tankB = tankAlgorithmFactory_->create(2, tanksList_.size());
                    tanksList_.push_back(std::move(tankB));
                    board_.placeTankB(coord, numShells);
                    satelliteView_->addPosition(row, col);
                    player2Tanks += 1;
                    tankPlayerIndices_.push_back(2);
                    break;
                }
                default:
                    logError("Unknown character '" + string(1, c) + "' at (" + to_string(col) + ", " + to_string(row) + "). Treated as space.");
                    break;
            }
        }
    }

    if (player1Tanks == 0 && player2Tanks == 0) {
        writeToOutputLog("Tie, both players have zero tanks");
        gameOver_ = true;
    }
    else if(player1Tanks == 0){
        writeToOutputLog("Player 2 won with " + std::to_string(player2Tanks) +" tanks still alive");
        gameOver_ = true;
    }
    else if (player2Tanks == 0)
    {
        writeToOutputLog("Player 1 won with " + std::to_string(player1Tanks) + " tanks still alive");
        gameOver_ = true;
    }

    if (hasErrors) { errorLog.close(); }
    
    if (gameOver_){
        return;
    }
    

    cout << "Board setup complete:\n";
    board_.render();


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
}

void GameManager::run() {
    if (gameOver_){
        return;
    }
    int rounds = 0;

    // by the advice given in the forum, we first move all the shells, check for hits,
    // move the tanks, and then check for hits again
    while ((rounds < maxRounds_ * 2) && (roundsWithoutAmmo_ < 80))
    {
        cout << "===Round " << rounds << "===" << endl;
        board_.render();
        board_.moveAllShells();
        std::vector<std::pair<size_t, size_t>> newTankPositions = board_.updateObjectsOnBoard();
        satelliteView_->setPositions(newTankPositions);
        satelliteView_->updateGrid(board_.getGrid());
        if (gameOver()){
            return;
        }
        if (rounds % 2 == 0){
            int tankInd = 0;
            ammoLeft_ = false;
            for (const auto& tank : tanksList_) {
                if (!board_.isTankAlive(tankInd)){
                    tankInd++;
                    continue;
                }
                satelliteView_->setCurrTankInd(tankInd);
                ActionRequest action = tank->getAction();
                bool actionSuccess = true;
                if (action != ActionRequest::GetBattleInfo){
                    actionSuccess = board_.act(action, tankInd);
                }
                else{
                    if (tankPlayerIndices_[tankInd] == 1){
                        player1_->updateTankWithBattleInfo(*tank, *satelliteView_);

                    }
                    else{
                        player2_->updateTankWithBattleInfo(*tank, *satelliteView_);
                    }
                }
                if (actionSuccess) {
                    writeToOutputLog(actionRequestToString(action));
                    cout << actionRequestToString(action) << endl;
                }
                else { 
                    writeToOutputLog(actionRequestToString(action) + " (ignored)");
                    cout << actionRequestToString(action) << "(ignored)" << endl;
                }
                if (board_.tankHasAmmo(tankInd)) { ammoLeft_ = true; }
                tankInd += 1;
            }
            
            std::vector<std::pair<size_t, size_t>> newTankPositions = board_.updateObjectsOnBoard();
            satelliteView_->setPositions(newTankPositions);
            satelliteView_->updateGrid(board_.getGrid());
        }
        if (gameOver()) { return; }
        if (!ammoLeft_) { roundsWithoutAmmo_++; }
        rounds += 1;
    }
    
    writeToOutputLog("Tie, reached max steps = " + std::to_string(maxRounds_) + ", player 1 has " + std::to_string(board_.getPlayerATanks()) + " tanks, player 2 has " + std::to_string(board_.getPlayerBTanks()) + " tanks");
}

bool GameManager::gameOver() {
    if (board_.getPlayerATanks() == 0 && board_.getPlayerBTanks() == 0) {
        writeToOutputLog("Tie, both players have zero tanks");
        gameOver_ = true;
    }
    else if(board_.getPlayerATanks() == 0){
        writeToOutputLog("Player 2 won with " + std::to_string(board_.getPlayerBTanks()) +" tanks still alive");
        gameOver_ = true;
    }
    else if (board_.getPlayerBTanks() == 0)
    {
        writeToOutputLog("Player 1 won with " + std::to_string(board_.getPlayerATanks()) +" tanks still alive");
        gameOver_ = true;
    }

    return gameOver_;
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
    outputFileName_.clear();
    string().swap(outputFileName_);
}

// taken from chatGPT
vector<string> GameManager::readInputFile(const string& filename) {
    ifstream infile(filename);
    vector<string> lines;

    if (!infile.is_open()) {
        throw runtime_error("Failed to open input file: " + filename);
    }

    string line;
    while (getline(infile, line)) {
        if (!line.empty()) {
            lines.push_back(line);
        }
    }

    return lines;
}

string GameManager::getOutputFilename(const string& inputFilename) {
    fs::path inputPath(inputFilename);
    string name = inputPath.filename().string();

    if (name.rfind("input_", 0) == 0 && name.size() > 6 && name.substr(name.size() - 4) == ".txt") {
        return "output_" + name.substr(6);  // Replace 'input_' with 'output_'
    } else {
        throw runtime_error("Invalid input file name: " + inputFilename + ". Expected format: input_x.txt");
    }
}

string GameManager::actionRequestToString(ActionRequest action) {
    switch (action) {
        case ActionRequest::MoveForward:    return "MoveForward";
        case ActionRequest::MoveBackward:   return "MoveBackward";
        case ActionRequest::RotateLeft90:   return "RotateLeft90";
        case ActionRequest::RotateRight90:  return "RotateRight90";
        case ActionRequest::RotateLeft45:   return "RotateLeft45";
        case ActionRequest::RotateRight45:  return "RotateRight45";
        case ActionRequest::Shoot:          return "Shoot";
        case ActionRequest::GetBattleInfo:  return "GetBattleInfo";
        case ActionRequest::DoNothing:      return "DoNothing";
        default:                            return "Unknown";
    }
}