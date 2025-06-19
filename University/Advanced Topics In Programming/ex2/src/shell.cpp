/**
 * @file shell.cpp
 * @brief A file detailing the implementation of all the functions declared in shell.h.
 * The maximal number of columns in a comment-line in this file is 106.
 */

#include "shell.h"
#include <algorithm>
using namespace std;

/* ------------------------------------------ CONSTRUCTORS: ------------------------------------------ */

Shell::Shell() {
    affectedPositions_ = vector<pair<int, int>>();
    affectedPositions_.reserve(8);
    pos_ = {0, 0};
    dir_ = Direction::U;
    exploded_ = false;
}

Shell::Shell(const vector<pair<int, int>>& affected_position, pair<int, int> pos, Direction dir,
    bool exploded) {
        affectedPositions_ = affected_position;
        pos_ = pos;
        dir_ = dir;
        exploded_ = exploded;
    }

Shell::Shell(const Shell& shell) {
    affectedPositions_ = shell.getAffectedPositions();
    pos_ = shell.getPos();
    dir_ = shell.getDir();
    exploded_ = shell.hasExploded();
}

Shell::Shell(Shell&& shell) noexcept {
    affectedPositions_ = shell.getAffectedPositions();
    pos_ = shell.getPos();
    dir_ = shell.getDir();
    exploded_ = shell.hasExploded();
    shell.resetShell();
}

/* -------------------------------------------- OPERATORS: ------------------------------------------- */

Shell& Shell::operator=(const Shell& shell) {
    if (this != &shell) {
        affectedPositions_ = shell.getAffectedPositions();
        pos_ = shell.getPos();
        dir_ = shell.getDir();
        exploded_ = shell.hasExploded();
    }
    return *this;
}

Shell& Shell::operator=(Shell&& shell) noexcept {
    if (this != &shell) {
        affectedPositions_ = shell.getAffectedPositions();
        pos_ = shell.getPos();
        dir_ = shell.getDir();
        exploded_ = shell.hasExploded();
        shell.resetShell();
    }
    return *this;
}

bool Shell::operator==(const Shell& shell) const {
    return (affectedPositions_ == shell.getAffectedPositions() && pos_ == shell.getPos() &&
            dir_ == shell.getDir() && exploded_ == shell.hasExploded());
}

bool Shell::operator!=(const Shell& shell) const {
    return !(*this == shell);
}

/* --------------------------------------------- GETTERS: -------------------------------------------- */

const vector<pair<int, int>>& Shell::getAffectedPositions() const {
    return affectedPositions_;
}

vector<pair<int, int>>& Shell::getAffectedPositions() {
    return affectedPositions_;
}

pair<int, int> Shell::getPos() const {
    return pos_;
}

Direction Shell::getDir() const {
    return dir_;
}

bool Shell::hasExploded() const {
    return exploded_;
}

/* --------------------------------------------- SETTERS: -------------------------------------------- */

void Shell::setAffectedPositions(vector<pair<int, int>> affected_positions) {
    affectedPositions_ = affected_positions;
}

void Shell::setPos(pair<int, int> coords) {
    pos_ = coords;
}

void Shell::setDir(Direction dir) {
    dir_ = dir;
}

void Shell::setExploded(bool exploded) {
    exploded_ = exploded;
}

/* ----------------------------------------- OTHER FUNCTIONS: ---------------------------------------- */

void Shell::addAffectedPosition(pair<int, int> coords) {
    affectedPositions_.push_back(coords);
}

void Shell::removeAffectedPosition(pair<int, int> coords) {
    affectedPositions_.erase(
        std::remove(affectedPositions_.begin(), affectedPositions_.end(), coords),
        affectedPositions_.end()
    );
}

void Shell::resetShell() {
    affectedPositions_.clear();
    vector<pair<int, int>>().swap(affectedPositions_);
    pos_ = pair<int, int>();
    dir_ = Direction::U;
    exploded_ = false;
}