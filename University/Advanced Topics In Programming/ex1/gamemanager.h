/**
 * @file gamemanager.h
 * @brief A file depicting the GameManager class declaration.
 * The maximal number of columns in a comment-line in this file is 106.
 */

#pragma once
#include "tankA.h"
#include "tankB.h"

class GameManager {
    private:
        Board board_;
        TankA tankA_;
        TankB tankB_;
        std::string outputFileName_;

    public:
        /* -------------------------------------- CONSTRUCTORS: -------------------------------------- */

        /** Default Constructor. */
        GameManager();
        /** Constructor with values for all fields. */
        GameManager(Board board, TankA tankA, TankB tankB, std::string output_file_name);
        /** Default Copy Constructor. */
        GameManager(const GameManager& game_manager);
        /** Default Move Constructor. */
        GameManager(GameManager&& game_manager) noexcept;
        /** Default Destructor. */
        ~GameManager() = default;

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
        /** Gets tankA itself. */
        TankA getTankA() const;
        /** Gets tankB itself. */
        TankB getTankB() const;
        /** Gets the output file's name. */
        std::string getOutputFileName() const;

        /* ----------------------------------------- SETTERS: ---------------------------------------- */

        /** Sets the board itself to the given one. */
        void setBoard(Board board);
        /** Sets tankA itself to the given one. */
        void setTankA(TankA tankA);
        /** Sets tankB itself to the given one. */
        void setTankB(TankB tankB);
        /** Sets the output file's name to the given one. */
        void setOutputFileName(std::string output_file_name);

        /* ------------------------------------- OTHER FUNCTIONS: ------------------------------------ */

        /** Sets up the board from the input lines. */
        void setupFromInput(const std::vector<std::string>& inputLines);
        /** Updates all objects on the board. */
        void updateObjectsOnBoard();
        /** Starts the game. */
        void startGame();
        /** Checks if the game is over. */
        bool gameOver();
        /** Writes the given message to the error log. */
        void writeToErrorLog(const std::string& message);
        /** Writes the given message to the output log. */
        void writeToOutputLog(const std::string& message);
        /** Resets the GameManaer-type object. */
        void resetGameManager();
};