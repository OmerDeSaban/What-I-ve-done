/**
 * @file tank.cpp
 * @brief A file detailing the implementation of all the functions declared in tank.h.
 * The maximal number of columns in a comment-line in this file is 106.
 */

#include "tank.h"
using namespace std;

/* ------------------------------------------ CONSTRUCTORS: ------------------------------------------ */

Tank::Tank() {
    pos_ = {0, 0};
    dir_ = Direction::U;
    ammo_ = 16;
    backCnt_ = -1;
    lastShotCnt_ = 0;
    alive_ = true;
    steppedOnAMine_ = false;
    hitByAShell_ = false;
    crashedWithOtherTank_ = false;
    playerInd_ = 1;
}

Tank::Tank(pair<int, int> pos, Direction dir, int playerInd, int ammo, int back_cnt, int last_shot_cnt, bool alive,
    bool stepped_on_a_mine, bool hit_by_a_shell, bool crashed_with_other_tank) {
        pos_ = pos;
        dir_ = dir;
        ammo_ = ammo;
        backCnt_ = back_cnt;
        lastShotCnt_ = last_shot_cnt;
        alive_ = alive;
        steppedOnAMine_ = stepped_on_a_mine;
        hitByAShell_ = hit_by_a_shell;
        crashedWithOtherTank_ = crashed_with_other_tank;
        playerInd_ = playerInd;
    }

Tank::Tank(const Tank& tank) {
    pos_ = tank.getPos();
    dir_ = tank.getDir();
    ammo_ = tank.getAmmo();
    backCnt_ = tank.getBackCnt();
    lastShotCnt_ = tank.getLastShotCnt();
    alive_ = tank.alive();
    steppedOnAMine_ = tank.steppedOnAMine();
    hitByAShell_ = tank.hitByAShell();
    crashedWithOtherTank_ = tank.crashedWithOtherTank();
    playerInd_ = tank.getPlayerInd();
}

Tank::Tank(Tank&& tank) noexcept {
    pos_ = tank.getPos();
    dir_ = tank.getDir();
    ammo_ = tank.getAmmo();
    backCnt_ = tank.getBackCnt();
    lastShotCnt_ = tank.getLastShotCnt();
    alive_ = tank.alive();
    steppedOnAMine_ = tank.steppedOnAMine();
    hitByAShell_ = tank.hitByAShell();
    crashedWithOtherTank_ = tank.crashedWithOtherTank();
    playerInd_ = tank.getPlayerInd();
    tank.resetTank();
}

/* -------------------------------------------- OPERATORS: ------------------------------------------- */

Tank& Tank::operator=(const Tank& tank) {
    if (this != &tank) {
        pos_ = tank.getPos();
        dir_ = tank.getDir();
        ammo_ = tank.getAmmo();
        backCnt_ = tank.getBackCnt();
        lastShotCnt_ = tank.getLastShotCnt();
        alive_ = tank.alive();
        steppedOnAMine_ = tank.steppedOnAMine();
        hitByAShell_ = tank.hitByAShell();
        crashedWithOtherTank_ = tank.crashedWithOtherTank();
    }
    return *this;
}

Tank& Tank::operator=(Tank&& tank) noexcept {
    if (this != &tank) {
        pos_ = tank.getPos();
        dir_ = tank.getDir();
        ammo_ = tank.getAmmo();
        backCnt_ = tank.getBackCnt();
        lastShotCnt_ = tank.getLastShotCnt();
        alive_ = tank.alive();
        steppedOnAMine_ = tank.steppedOnAMine();
        hitByAShell_ = tank.hitByAShell();
        crashedWithOtherTank_ = tank.crashedWithOtherTank();
        tank.resetTank();
    }
    return *this;
}

bool Tank::operator==(const Tank& tank) const {
    return (pos_ == tank.getPos() && dir_ == tank.getDir() && ammo_ == tank.getAmmo() &&
        backCnt_ == tank.getBackCnt() && lastShotCnt_ == tank.getLastShotCnt() &&
        alive_ == tank.alive() && steppedOnAMine_ == tank.steppedOnAMine() &&
        hitByAShell_ == tank.hitByAShell() && crashedWithOtherTank_ == tank.crashedWithOtherTank());
}

bool Tank::operator!=(const Tank& tank) const {
    return !(*this == tank);
}

/* --------------------------------------------- GETTERS: -------------------------------------------- */

pair<int, int> Tank::getPos() const {
    return pos_;
}

Direction Tank::getDir() const {
    return dir_;
}

int Tank::getAmmo() const {
    return ammo_;
}

int Tank::getBackCnt() const {
    return backCnt_;
}

int Tank::getLastShotCnt() const {
    return lastShotCnt_;
}

bool Tank::alive() const {
    return alive_;
}

bool Tank::steppedOnAMine() const {
    return steppedOnAMine_;
}

bool Tank::hitByAShell() const {
    return hitByAShell_;
}

bool Tank::crashedWithOtherTank() const {
    return crashedWithOtherTank_;
}

int Tank::getPlayerInd() const {
    return playerInd_;
}

/* --------------------------------------------- SETTERS: -------------------------------------------- */

void Tank::setPos(pair<int, int> pos) {
    pos_ = pos;
}

void Tank::setDir(Direction dir) {
    dir_ = dir;
}

void Tank::setAmmo(int ammo) {
    ammo_ = ammo;
}

void Tank::setBackCnt(int back_cnt) {
    backCnt_ = back_cnt;
}

void Tank::setLastShotCnt(int last_shot_cnt) {
    lastShotCnt_ = last_shot_cnt;
}

void Tank::setAlive(bool alive) {
    alive_ = alive;
}

void Tank::setSteppedOnAMine(bool stepped_on_a_mine) {
    steppedOnAMine_ = stepped_on_a_mine;
}

void Tank::setHitByAShell(bool hit_by_a_shell) {
    hitByAShell_ = hit_by_a_shell;
}

void Tank::setCrashedWithOtherTank(bool crashed_with_other_tank) {
    crashedWithOtherTank_ = crashed_with_other_tank;
}

/* ------------------------------------- OTHER FUNCTIONS: ------------------------------------ */

void Tank::decreaseAmmo() {
    ammo_ -= 1;
}

void Tank::decreaseBackCnt() {
    backCnt_ -= 1;
}

void Tank::resetBackCnt() {
    backCnt_ = -1;
}

void Tank::decreaseLastShotCount() {
    if (lastShotCnt_ > 0){
        lastShotCnt_ -= 1;
    }
}

void Tank::resetLastShotCnt() {
    lastShotCnt_ = 0;
}

void Tank::KILL() {
    alive_ = false;
}

void Tank::stepOnAMine() {
    steppedOnAMine_ = true;
    alive_ = false;
}

void Tank::kaboom() {
    hitByAShell_ = true;
    alive_ = false;
}

void Tank::batteringRam() {
    crashedWithOtherTank_ = true;
    alive_ = false;
}

string Tank::getCauseOfDeath() {
    string causeOfDeath = "cause of death: ";
    if (steppedOnAMine_) {
        causeOfDeath += "stepped on a mine, ";
    }
    if(hitByAShell_) {
        causeOfDeath += "hit by a shell, ";
    }
    if(crashedWithOtherTank_) {
        causeOfDeath += "crashed with the other tank.";
    }
    return causeOfDeath;
}

bool Tank::canShoot(){
    return (ammo_ > 0 && lastShotCnt_ == 0);
}

bool Tank::shouldWait(ActionRequest action) {
    return backCnt_ > 0 && !(action == ActionRequest::MoveForward || action == ActionRequest::MoveBackward);
}


void Tank::resetTank() {
    pos_ = pair<int, int>();
    dir_ = Direction::U;
    ammo_ = 0;
    backCnt_ = 0;
    lastShotCnt_ = 0;
    alive_ = false;
    steppedOnAMine_ = false;
    hitByAShell_ = false;
    crashedWithOtherTank_ = false;
}