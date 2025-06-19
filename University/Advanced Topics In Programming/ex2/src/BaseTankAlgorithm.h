#pragma once
#include "TankAlgorithm.h"
#include "BaseBattleInfo.h"
#include "direction.h"
#include <vector>
#include <utility>

class BaseTankAlgorithm : public TankAlgorithm {
protected:
    int player_index_;
    int tank_index_;
    Direction dir_;
    int ammo_;
    std::pair<int, int> pos_;
    bool relevantInfo_;
    std::vector<std::vector<char>> grid_;
    std::vector<std::pair<int, int>> enemy_positions_;
    int lastShotCnt_;

public:
    BaseTankAlgorithm(int player_index, int tank_index);

    int getPlayerInd() const;

    void updateBattleInfo(BattleInfo& info) override;

    std::pair<int, int> getNextPos(const std::vector<std::pair<int, int>>& path);
    void findMyPos();
    Direction getDirectionToward(pair<int, int> coords);
    ActionRequest whichRotation(Direction desired_dir);
    bool isSafe(pair<int, int> pos);
    bool canMoveForward();
    bool canMoveBackward();
    bool hasLineOfSight(pair<int, int> pos, Direction dir, pair<int, int> target_pos);
    ActionRequest findRotationForLineOfSight(pair<int, int> target_pos);
    void findEnemyPositions();

    // Getters
    int getPlayerIndex() const { return player_index_; }
    int getTankIndex() const { return tank_index_; }
    Direction getDir() const { return dir_; }
    int getAmmo() const { return ammo_; }
    std::pair<int, int> getPos() const { return pos_; }
    bool getRelevantInfo() const { return relevantInfo_; }
    std::vector<std::vector<char>> getGrid() const { return grid_; }
    std::vector<std::pair<int, int>> getEnemyPositions() const { return enemy_positions_; }
    int getLastShotCnt() const { return lastShotCnt_; }

    // Setters
    void setPlayerIndex(int index) { player_index_ = index; }
    void setTankIndex(int index) { tank_index_ = index; }
    void setDir(Direction dir) { dir_ = dir; }
    void setAmmo(int ammo) { ammo_ = ammo; }
    void setPosition(std::pair<int, int> pos) { pos_ = pos; }
    void setRelevantInfo(bool info) { relevantInfo_ = info; }
    void setGrid(const std::vector<std::vector<char>>& grid) { grid_ = grid; }
    void setEnemyPositions(const std::vector<std::pair<int, int>>& enemies) { enemy_positions_ = enemies; }
    void resetLastShotCount(){ lastShotCnt_ = 4; }
    void decreaseLastShotCount() { if (lastShotCnt_ > 0){ lastShotCnt_ -= 1; } }
    void decreaseAmmo() { if (ammo_ > 0){ ammo_ -= 1; } }
    bool canShoot() { return ((ammo_ > 0) && (lastShotCnt_ == 0)); }
    void rotate(ActionRequest action);
    bool isPathSafe(const std::vector<std::pair<int, int>>& path);
};