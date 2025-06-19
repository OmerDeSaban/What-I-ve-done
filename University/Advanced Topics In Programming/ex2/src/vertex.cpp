/**
 * @file vertex.cpp
 * @brief A file detailing the implementation of all the functions declared in vertex.h.
 * The maximal number of columns in a comment-line in this file is 106.
 */

#include "vertex.h"
using namespace std;

/* ------------------------------------------ CONSTRUCTORS: ------------------------------------------ */

Vertex::Vertex() {
    adj_ = vector<pair<int, int>>();
    adj_.reserve(8);
    X_ = 0;
    Y_ = 0;
    HP_ = 0;
    isWall_ = false;
    isMine_ = false;
}

Vertex::Vertex(const vector<pair<int, int>>& adj, int X, int Y, int HP, bool is_wall, bool is_mine) {
    adj_ = adj;
    X_ = X;
    Y_ = Y;
    HP_ = HP;
    isWall_ = is_wall;
    isMine_ = is_mine;
}

Vertex::Vertex(const Vertex& vertex) {
    adj_ = vertex.getAdj();
    X_ = vertex.getX();
    Y_ = vertex.getY();
    HP_ = vertex.getHP();
    isWall_ = vertex.isWall();
    isMine_ = vertex.isMine();
}

Vertex::Vertex(Vertex&& vertex) noexcept {
    adj_ = move(vertex.adj_);
    X_ = vertex.X_;
    Y_ = vertex.Y_;
    HP_ = vertex.HP_;
    isWall_ = vertex.isWall_;
    isMine_ = vertex.isMine_;
    vertex.resetVertex();
}

/* -------------------------------------------- OPERATORS: ------------------------------------------- */

Vertex& Vertex::operator=(const Vertex& vertex) {
    if (this != &vertex) {
        adj_ = vertex.getAdj();
        X_ = vertex.getX();
        Y_ = vertex.getY();
        HP_ = vertex.getHP();
        isWall_ = vertex.isWall();
        isMine_ = vertex.isMine();
    }
    return *this;
}

Vertex& Vertex::operator=(Vertex&& vertex) noexcept {
    if (this != &vertex) {
        adj_ = move(vertex.adj_);
        X_ = vertex.getX();
        Y_ = vertex.getY();
        HP_ = vertex.getHP();
        isWall_ = vertex.isWall();
        isMine_ = vertex.isMine();
        vertex.resetVertex();
    }
    return *this;
}

bool Vertex::operator==(const Vertex& vertex) const {
    return (adj_ == vertex.getAdj() && X_ == vertex.getX() && Y_ == vertex.getY() &&
            HP_ == vertex.getHP() && isWall_ == vertex.isWall() && isMine_ == vertex.isMine());
}

bool Vertex::operator!=(const Vertex& vertex) const {
    return !(*this == vertex);
}

/* --------------------------------------------- GETTERS: -------------------------------------------- */

const vector<pair<int, int>>& Vertex::getAdj() const noexcept {
    return adj_;
}

int Vertex::getX() const noexcept {
    return X_;
}

int Vertex::getY() const noexcept {
    return Y_;
}

pair<int, int> Vertex::getCoords() const noexcept {
    return make_pair(X_, Y_);
}

int Vertex::getHP() const noexcept {
    return HP_;
}

bool Vertex::isWall() const noexcept {
    return isWall_;
}

bool Vertex::isMine() const noexcept {
    return isMine_;
}

/* --------------------------------------------- SETTERS: -------------------------------------------- */

void Vertex::setAdj(const vector<pair<int, int>>& adj) {
    adj_ = adj;
}

void Vertex::setX(int x) {
    if (x < 0) { throw invalid_argument("A vertex's X coordinate cannot be negative!"); }
    X_ = x;
}

void Vertex::setY(int y) {
    if (y < 0) { throw invalid_argument("A vertex's Y coordinate cannot be negative!"); }
    Y_ = y;
}

void Vertex::setHP(int hp) {
    if (hp < 0) { throw invalid_argument("A wall's remaining HP cannot be negative!"); }
    else if (hp > 2) {
        throw invalid_argument("A wall's remaining HP cannot be greater than 2!");
    }
    HP_ = hp;
}

void Vertex::setWall(bool is_wall) {
    isWall_ = is_wall;
}

void Vertex::setMine(bool is_mine) {
    isMine_ = is_mine;
}

/* ----------------------------------------- OTHER FUNCTIONS: ---------------------------------------- */

void Vertex::addAdjCoords(pair<int, int> coords) {
    adj_.push_back(coords);
}

void Vertex::clearAdj() noexcept {
    vector<pair<int, int>>().swap(adj_);
}

pair<int, int> Vertex::getAdjCoordInDirection(Direction dir) const {
    int newVertexInd = directionToIndex(dir);
    if (adj_.empty() || newVertexInd < 0 || newVertexInd >= static_cast<int>(adj_.size())) {
        throw out_of_range("Direction index out of range or no adjacent coordinates");
    }
    return adj_[newVertexInd];
}

void Vertex::increaseHP() {
    if (!isWall_) throw invalid_argument("Cannot increase the HP of a non-wall vertex!");
    if (HP_ >= 2) throw invalid_argument("Wall HP cannot exceed 2!");
    HP_++;
}

void Vertex::decreaseHP() {
    if (!isWall_) throw invalid_argument("Cannot decrease the HP of a non-wall vertex!");
    if (HP_ <= 0) throw invalid_argument("Wall HP cannot go below 0!");
    HP_--;
}

bool Vertex::shouldWreckWall() {
    return (HP_ > 0 && isWall_);
}

void Vertex::wreckWall() noexcept {
    isWall_ = false;
    HP_ = 0;
}

void Vertex::resetVertex() noexcept {
    adj_.clear();
    X_ = Y_ = HP_ = 0;
    isWall_ = isMine_ = false;
}