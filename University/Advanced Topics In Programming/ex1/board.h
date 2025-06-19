/**
 * @file board.h
 * @brief A file depicting the Board class declaration.
 * The maximal number of columns in a comment-line in this file is 106.
 */

#pragma once
#include <iostream>
#include "vertex.h"
#include "bfs.h"
#include "shell.h"

class Board {
    private:
        int rows_;
        int columns_;
        std::vector<std::vector<std::string>> grid_;
        std::map<std::pair<int, int>, Vertex> vertices_;
        std::vector<Shell> shells_;
        BFS bfs_;
        std::pair<int, int> posTankA_;
        std::pair<int, int> posTankB_;

    public:
        /* -------------------------------------- CONSTRUCTORS: -------------------------------------- */

        /** Default constructor. */
        Board();
        /** Constructor with values ONLY for rows_ and columns_. */
        Board(int rows, int columns);
        /** Constructor with values for all fields. */
        Board(int rows, int columns, const std::vector<std::vector<std::string>>& grid,
            const std::map<std::pair<int, int>, Vertex>& vertices, const std::vector<Shell>& shells,
            BFS bfs, std::pair<int, int> tankA_pos, std::pair<int, int> tankB_pos);
        /** Default Copy Constructor. */
        Board(const Board& board);
        /** Default Move Constructor. */
        Board(Board&& board) noexcept;
        /** Default Deconstructor. */
        ~Board() = default;

        /* ---------------------------------------- OPERATORS: --------------------------------------- */

        /** Default Assignment Operator. */
        Board& operator=(const Board& board);
        /** Default Move Assignment Operator. */
        Board& operator=(Board&& board) noexcept;
        /** Default Equality Operator. */
        bool operator==(const Board& board) const;
        /** Default Inequality Operator. */
        bool operator!=(const Board& board) const;

        /* ----------------------------------------- GETTERS: ---------------------------------------- */

        /** Gets the number of rows in the board. */
        int getRows() const;
        /** Gets the number of columns in the board. */
        int getColumns() const;
        /** Gets the grid matrix that describes the board's state, without the ability to modify it. */
        const std::vector<std::vector<std::string>>& getGrid() const;
        /** Gets the grid matrix that describes the board's state, with the ability to modify it. */
        std::vector<std::vector<std::string>>& getGrid();
        /** Gets the map of all the vertices the board consists of. */
        const std::map<std::pair<int, int>, Vertex>& getVertices() const;
        /** Gets the vector denoting all shells on the board. */
        const std::vector<Shell>& getShells() const;
        /** Gets a non-const version of the vector denoting all shells on the board. */
        std::vector<Shell>& getShells();
        /** Gets the BFS that was run on the board between some position of tankA, and some position of
         * tankB. */
        BFS getBFS() const;
        /** Gets the (X, Y) coordinates of tankA on the board. */
        std::pair<int, int> getPosTankA() const;
        /** Gets the (X, Y) coordinates of tankB on the board. */
        std::pair<int, int> getPosTankB() const;

        /* ----------------------------------------- SETTERS: ---------------------------------------- */

        /** Sets the number of rows in the board to the given integer. */
        void setRows(int rows);
        /** Sets the number of columns in the board to the given integer. */
        void setColumns(int columns);
        /** Sets the grid matrix that describes the board's state to the given one. */
        void setGrid(const std::vector<std::vector<std::string>>& grid);
        /** Sets the map of all the vertices the board consists of to the given one. */
        void setVertices(const std::map<std::pair<int, int>, Vertex>& vertices);
        /** Sets the vector denoting all shells on the board to the given one. */
        void setShells(const std::vector<Shell>& shells);
        /** Sets the BFS that was run on the board between some position of tankA, and some position of
         * tankB to the given one. */
        void setBFS(BFS bfs);
        /** Sets the (X, Y) coordinates of tankA on the board to the given pair of (X, Y) integer. */
        void setPosTankA(const std::pair<int, int> tankA_pos);
        /** Sets the (X, Y) coordinates of tankB on the board to the given pair of (X, Y) integer. */
        void setPosTankB(const std::pair<int, int> tankB_pos);

        /* ------------------------------------- OTHER FUNCTIONS: ------------------------------------ */

        /** Prints the board's current state to the console. */
        void render() const;
        /** Gets the X coordinate of an (X, Y) coordinate's neighbor in the given direction.  */
        int getNeighborXCoord(int currentX, int deltaX);
        /** Gets the Y coordinate of an (X, Y) coordinate's neighbor in the given direction.  */
        int getNeighborYCoord(int currentY, int deltaY);
        /** Gets the coordinates of an (X, Y) coordinate's neighbor in the given direction. */
        std::pair<int, int> getNeighborCoords(std::pair<int, int> current_pos, Direction dir);
        /** Places tankA on the board. */
        void placeTankA(std::pair<int, int> pos);
        /** Places tankB on the board. */
        void placeTankB(std::pair<int, int> pos);
        /** Places a wall on the board. */
        void placeWall(std::pair<int, int> pos);
        /** Places a mine on the board. */
        void placeMine(std::pair<int, int> pos);
        /** Creates a new vertex and adds it to the map of all the vertices the board consists of. */
        void addNewVertexToMap(int X, int Y, bool is_wall = false, bool is_mine = false);
        /** Adds a pre-created vertex to the map of all the vertices the board consists of. */
        void addExistingVertexToMap(Vertex vertex);
        /** Initializes the board. */
        void initializeBoard();
        /** Creates a new shell at the given (X, Y) coordinates, which moves in the given direction, and
         * adds it to the vector denoting all shells on the board. */
        void addNewShell(int X, int Y, Direction dir);
        /** Removes an existing shell from the vector depicting all shells on the board, and destroying
         * it. */
        void removeShell(const Shell& shell);
        /** Checks if the BFS path is safe. */
        bool isBFSPathSafe();
        /** Moves a shell on the board. */
        void moveShell(Shell& shell);
        /** Moves all the shells on the board. */
        void moveAllShells();
        /** Checks if a certain position on the board is already occupied. */
        bool placeTaken(std::pair<int, int> pos);
        /** Checks if a certain position on the board would be safe for the next 8 turns (4 tank
         * turns). */
        bool isSafe(std::pair<int, int> pos);
        /** Checks if 2 objects would intersect during their current movement. */
        bool intersect(std::pair<int, int> objectA_start, std::pair<int, int> objectA_des,
            std::pair<int, int> objectB_start, std::pair<int, int> objectB_des);
        /** Removes a tank at the given (X, Y) coordinates from the grid. */
        void removeTankFromGrid(std::pair<int, int> pos);
        /** Resets the Board-type object. */
        void resetBoard();
};