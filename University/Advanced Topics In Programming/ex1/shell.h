/**
 * @file shell.h
 * @brief A file depicting the Shell class declaration.
 * The maximal number of columns in a comment-line in this file is 106.
 */

#pragma once
#include <vector>
#include "direction.h"

class Shell {
    private:
        std::vector<std::pair<int, int>> affectedPositions_;
        std::pair<int, int> pos_;
        Direction dir_;
        bool exploded_;
    
    public:
        /* -------------------------------------- CONSTRUCTORS: -------------------------------------- */

        /** Default constructor. */
        Shell();
        /** Constructor with values for all fields. */
        Shell(const std::vector<std::pair<int, int>>& affected_position, std::pair<int, int> pos, Direction dir,
            bool exploded);
        /** Default Copy Constructor. */
        Shell(const Shell& shell);
        /** Default Move Constructor. */
        Shell(Shell&& shell) noexcept;
        /** Default Deconstructor. */
        ~Shell() = default;

        /* ---------------------------------------- OPERATORS: --------------------------------------- */

        /** Default Assignment Operator. */
        Shell& operator=(const Shell& shell);
        /** Default Move Assignment Operator. */
        Shell& operator=(Shell&& shell) noexcept;
        /** Default Equality Operator. */
        bool operator==(const Shell& shell) const;
        /** Default Inequality Operator. */
        bool operator!=(const Shell& shell) const;

        /* ----------------------------------------- GETTERS: ---------------------------------------- */

        /** Gets the vector of (X, Y) coordinates marking which vertices the Shell is destined to hit,
         * but without the ability to modify it. */
        const std::vector<std::pair<int, int>>& getAffectedPositions() const;
        /** Gets the vector of (X, Y) coordinates marking which vertices the Shell is destined to hit,
         * with the ability to modify it. */
        std::vector<std::pair<int, int>>& getAffectedPositions();
        /** Gets the shell's (X, Y) coordinates. */
        std::pair<int, int> getPos() const;
        /** Gets the shell's direction. */
        Direction getDir() const;
        /** Checks if the shell has exploded. */
        bool hasExploded() const;

        /* ----------------------------------------- SETTERS: ---------------------------------------- */

        /** Gets the vector of (X, Y) coordinates marking which vertices the shell is destined to hit to
         * the given one. */
        void setAffectedPositions(std::vector<std::pair<int, int>> affected_positions);
        /** Sets the shell's (X, Y) coordinates to the given pair of (X, Y) integers. */
        void setPos(std::pair<int, int> coords);
        /** Sets the shell's direction to the given one. */
        void setDir(Direction dir);
        /** Sets the shell's explosion status to the given one. */
        void setExploded(bool exploded);

        /* ------------------------------------- OTHER FUNCTIONS: ------------------------------------ */

        /** Adds a pair of (X, Y) integers to the vector of (X, Y) coordinates marking which vertices the
         * shell is destined to hit */
        void addAffectedPosition(std::pair<int, int> coords);
        /** Removes a pair of (X, Y) integers from the vector of (X, Y) coordinates marking which
         * vertices the shell is destined to hit */
        void removeAffectedPosition(std::pair<int, int> coords);
        /** Resets the Shell-type object. */
        void resetShell();
};