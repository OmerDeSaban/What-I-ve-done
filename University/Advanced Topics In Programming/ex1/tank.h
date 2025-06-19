/**
 * @file tank.h
 * @brief A file depicting the Tank class declaration.
 * The maximal number of columns in a comment-line in this file is 106.
 */

#pragma once
#include "board.h"
#include "actions.h"

class Tank {
    private:
        std::pair<int, int> pos_;
        Direction dir_;
        int ammo_;
        int backCnt_;
        int lastShotCnt_;
        bool alive_;
        bool steppedOnAMine_;
        bool hitByAShell_;
        bool crashedWithOtherTank_;
    
    public:
        /* -------------------------------------- CONSTRUCTORS: -------------------------------------- */

        /** Default Constructor. */
        Tank();
        /** Constructor with values for all fields. */
        Tank(std::pair<int, int> pos, Direction dir, int ammo = 16, int back_cnt = -1,
            int last_shot_cnt = 0, bool alive = true, bool stepped_on_a_mine = false,
            bool hit_by_a_shell = false, bool crashed_with_other_tank = false);
        /** Default Copy Constructor. */
        Tank(const Tank& tank);
        /** Default Move Constructor. */
        Tank(Tank&& tank) noexcept;
        /** Default Deconstructor. */
        ~Tank() = default;

        /* ---------------------------------------- OPERATORS: --------------------------------------- */

        /** Default Assignment Operator. */
        Tank& operator=(const Tank& tank);
        /** Default Move Assignment Operator. */
        Tank& operator=(Tank&& tank) noexcept;
        /** Default Equality Operator. */
        bool operator==(const Tank& tank) const;
        /** Default Inequality Operator. */
        bool operator!=(const Tank& tank) const;

        /* ----------------------------------------- GETTERS: ---------------------------------------- */

        /** Gets the (X, Y) coordinates marking the tank's position. */
        std::pair<int, int> getPos() const;
        /** Gets the direction of the tank's turret. */
        Direction getDir() const;
        /** Checks how much ammo the tank has left. */
        int getAmmo() const;
        /** Checks how many tank turns the tank has waited since the backward move request. */
        int getBackCnt() const;
        /** Checks how many tank turns have passed since the tank last shot its turret. */
        int getLastShotCnt() const;
        /** Checks if the tank is still alive. */
        bool alive() const;
        /** Checks if the tank stepped on a mine. */
        bool steppedOnAMine() const;
        /** Checks if the tank was hit by a shell. */
        bool hitByAShell() const;
        /** Chekcs if the tank has crashed with the other tank. */
        bool crashedWithOtherTank() const;

        /* ----------------------------------------- SETTERS: ---------------------------------------- */

        /** Sets the (X, Y) coordinates marking the tank's position to the given pair of (X, Y)
         * integers. */
        void setPos(std::pair<int, int> pos);
        /** Sets the direction of the tank's turret to the given one. */
        void setDir(Direction dir);
        /** Sets the tank's remaining ammo counter to the given integet. */
        void setAmmo(int ammo);
        /** Sets the number of tank turns the tank has waited since the backward move request to the
         * given integer. */
        void setBackCnt(int back_cnt);
        /** Sets the number of tank turns have passed since the tank last shot its turret to the given
         * integer. */
        void setLastShotCnt(int last_shot_cnt);
        /** Sets the tank's alive status to the given one. */
        void setAlive(bool alive);
        /** Sets the tank's mine-step status to the given one. */
        void setSteppedOnAMine(bool stepped_on_a_mine);
        /** Sets the tank's hit-by-a-shell status to the given one. */
        void setHitByAShell(bool hit_by_a_shell);
        /** Sets the tank's crashed-with-other-tank status to the given one. */
        void setCrashedWithOtherTank(bool crashed_with_other_tank);

        /* ------------------------------------- OTHER FUNCTIONS: ------------------------------------ */

        /** Lowers the tank's remaining ammo count by 1. */
        void decreaseAmmo();
        /** Lowers the tank's tank turn counter since the backward move request by 1. */
        void decreaseBackCnt();
        /** Resets the tank's tank turn counter since the backward move request. */
        void resetBackCnt();
        /** Lowers the tank's tank turn counter since the last shot it fired by 1. */
        void decreaseLastShotCount();
        /** Resets the tank's tank turn counter since the last shot it fired. */
        void resetLastShotCnt();
        /** Kills the tank. */
        void KILL();
        /** Deals with the case where the tank steps on a mine. */
        void stepOnAMine();
        /** Deals with the case where the tank gets hit by a shell. */
        void kaboom();
        /** Deals with the case where the tank crashes with the other tank. */
        void batteringRam();
        /** Checks why the tank was destroyed. */
        std::string getCauseOfDeath();
        /** Finds the (X, Y) coordinates of the tank's next position based on its current movement
         * vector. */
        std::pair<int, int> getNextPos(const std::vector<std::pair<int, int>>& path);
        /** Finds the direction the tank should turn towards from the current position to another. */
        Direction getDirectionToward(std::pair<int, int> coords, Board& board);
        /** Checks which rotation the tank should make in order to move towards a certain direction from
         * its current direction. */
        Action whichRotation(Direction desired_dir);
        /** Checks if the tank can move forwards. */
        bool canMoveForward(Board& board);
        /** Checks if the tank can move backwards. */
        bool canMoveBackward(Board& board);
        /** Checks if the tank has a line of sight towards a given target position, while looking at a
         * certain direction. */
        bool hasLineOfSight(pair<int, int> pos, Direction dir, std::pair<int, int> target_pos,
            Board& board);
        /** Finds which rotation the tank should do to get a line of sight towards a given target
         * position. */
        Action findRotationForLineOfSight(std::pair<int, int> target_pos, Board& board);
        /* returns true if tank can shoot in terms of ammo and wait period */
        bool canShoot();
        /** Deals with tank's movement actions. Returns true if action was successful. */
        bool move(Action action, Board& board);
        /** Deals with tank's shooting actions. Returns true if action was successful. */
        bool shoot();
        /** Deals with tank's rotation actions. Returns true if action was successful. */
        bool rotate(Action action);
        /** Checks if the tank's should wait due to a backward move request. */
        bool shouldWait(Action action);
        /** Determines what action the tank should take next. */
        virtual Action nextAction(Board& board) = 0;
        /** Does the tank's desired action. */
        bool act(Action action, Board& board);
        /** Resets the Tank-type object. */
        void resetTank();
};