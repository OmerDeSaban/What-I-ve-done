/**
 * @file gamemanager.h
 * @brief A file depicting the GameManager class declaration.
 * The maximal number of columns in a comment-line in this file is 106.
 */

#pragma once
#include "board.h"
#include "TankAlgorithmFactory.h"
#include "PlayerFactory.h"
#include "Player.h"
#include "TankAlgorithm.h"
#include "ActionRequest.h"
#include "BaseSatelliteView.h"

class GameManager {
    private:
        Board board_;
        std::string outputFileName_;
        std::unique_ptr<PlayerFactory> playerFactory_;
        std::unique_ptr<TankAlgorithmFactory> tankAlgorithmFactory_;
        std::unique_ptr<Player> player1_;
        std::unique_ptr<Player> player2_;
        std::vector<std::unique_ptr<TankAlgorithm>> tanksList_;
        bool gameOver_;
        int maxRounds_;
        std::unique_ptr<BaseSatelliteView> satelliteView_;
        std::vector<int> tankPlayerIndices_;
        bool ammoLeft_;
        int roundsWithoutAmmo_;

    public:
        /* -------------------------------------- CONSTRUCTORS: -------------------------------------- */
        /** constructor for assignment 2 */
        GameManager(std::unique_ptr<PlayerFactory> playerFactory, std::unique_ptr<TankAlgorithmFactory> tankAlgorithmFactory);

        /* ---------------------------------------- OPERATORS: --------------------------------------- */

        /** Default Assignment Operator. */
        GameManager& operator=(const GameManager& game_manager);
        /** Default Move Assignment Operator. */
        GameManager& operator=(GameManager&& game_manager) noexcept;
        /** Default Equality Operator. */
        bool operator==(const GameManager& game_manager) const;
        /** Default Inequality Operator. */
        bool operator!=(const GameManager& game_manager) const;

        /* ----------------------------------------- GETTERS: ---------------------------------------- */

        /** Gets the board itself. */
        Board getBoard() const;

        /** Gets the output file's name. */
        std::string getOutputFileName() const;

        /* ----------------------------------------- SETTERS: ---------------------------------------- */

        /** Sets the board itself to the given one. */
        void setBoard(Board board);

        /** Sets the output file's name to the given one. */
        void setOutputFileName(std::string output_file_name);

        /* ------------------------------------- OTHER FUNCTIONS: ------------------------------------ */

        /** Sets up the board from the input lines. */
        void readBoard(std::string input_file_name);
        /** Starts the game. */
        void run();
        /** Checks if the game is over. */
        bool gameOver();
        /** Writes the given message to the error log. */
        void writeToErrorLog(const std::string& message);
        /** Writes the given message to the output log. */
        void writeToOutputLog(const std::string& message);
        /** Resets the GameManaer-type object. */
        void resetGameManager();
        /** utils */
        vector<string> readInputFile(const string& filename);
        string getOutputFilename(const string& inputFilename);
        string actionRequestToString(ActionRequest action);
};