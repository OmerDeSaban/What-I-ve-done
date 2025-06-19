/**
 * @file vertex.h
 * @brief A file depicting the Vertex class declaration.
 * The maximal number of columns in a comment-line in this file is 106.
 */

#pragma once
#include <vector>
#include "direction.h"

class Vertex {
    private:
        std::vector<std::pair<int, int>> adj_;
        int X_;
        int Y_;
        int HP_;
        bool isWall_;
        bool isMine_;

    public:
        /* -------------------------------------- CONSTRUCTORS: -------------------------------------- */

        /** Default constructor. */
        Vertex();
        /** Constructor with values for all fields. */
        Vertex(const std::vector<std::pair<int, int>>& adj, int X, int Y, int HP, bool is_wall, bool is_mine);
        /** Default Copy Constructor. */
        Vertex(const Vertex& vertex);
        /** Default Move Constructor. */
        Vertex(Vertex&& vertex) noexcept;
        /** Default Deconstructor. */
        ~Vertex() = default;

        /* ---------------------------------------- OPERATORS: --------------------------------------- */

        /** Default Assignment Operator. */
        Vertex& operator=(const Vertex& vertex);
        /** Default Move Assignment Operator. */
        Vertex& operator=(Vertex&& vertex) noexcept;
        /** Default Equality Operator. */
        bool operator==(const Vertex& vertex) const;
        /** Default Inequality Operator. */
        bool operator!=(const Vertex& vertex) const;

        /* ----------------------------------------- GETTERS: ---------------------------------------- */

        /** Gets the (X, Y) coordinates of vertices adjacent to this one. */
        const std::vector<std::pair<int, int>>& getAdj() const noexcept;
        /** Gets the vertex's X coordinate. */
        int getX() const noexcept;
        /** Gets the vertex's Y coordinate. */
        int getY() const noexcept;
        /** Gets the vertex's (X, Y) coordinates as a pair of integers. */
        std::pair<int, int> getCoords() const noexcept;
        /** Gets the wall's remaining HP. */
        int getHP() const noexcept;
        /** Checks if this vertex is a wall. */
        bool isWall() const noexcept;
        /** Checks if this vertex is a mine. */
        bool isMine() const noexcept;

        /* ----------------------------------------- SETTERS: ---------------------------------------- */

        /** Sets the vector of (X, Y) coordinates of this vertex's of adjacent vertices to the given
         * one. */
        void setAdj(const std::vector<std::pair<int, int>>& adj);
        /** Sets the vertex's X coordinate to the given integer. Must be non-negative. */
        void setX(int x);
        /** Sets the vertex's Y coordinate to the given integer. Must be non-negative. */
        void setY(int y);
        /** Sets the wall's remaining HP to the given integer. Must be non-negative, and not greater than
         * 2. */
        void setHP(int hp);
        /** Sets the vertex's wall status to the given one. */
        void setWall(bool is_wall);
        /** Sets the vertex's mine status to the given one.. */
        void setMine(bool is_mine);

        /* ------------------------------------- OTHER FUNCTIONS: ------------------------------------ */

        /** Adds the (X, Y) coordinates of a new vertex to the vector of (X, Y) coordinates of this
         * vertex's vector of adjacent vertices */
        void addAdjCoords(std::pair<int, int> coords);
        /** Clears all memory allocated for the vertex's adj_ field. */
        void clearAdj() noexcept;
        /** Gets the (X, Y) coordinates of this vertex's adjacent vertex in the specified direction */
        std::pair<int, int> getAdjCoordInDirection(Direction dir) const;
        /** Increases the wall's remaining HP by 1. */
        void increaseHP();
        /** Decreases the wall's remaining HP by 1. */
        void decreaseHP();
        /** Checks if this vertex is a wall with remaining HP. */
        bool shouldWreckWall();
        /** Wrecks a wall. */
        void wreckWall() noexcept;
        /** Resets the Vertex-type object. */
        void resetVertex() noexcept;
};