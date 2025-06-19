/**
 * @file bfs_vertex.cpp
 * @brief A file detailing the implementation of all the functions declared in bfs_vertex.h.
 * The maximal number of columns in a comment-line in this file is 106.
 */

#include "bfs_vertex.h"
#include <stdexcept>
using namespace std;

/* ------------------------------------------ CONSTRUCTORS: ------------------------------------------ */

BFS_Vertex::BFS_Vertex() {
    vertexPos_ = {0, 0};
    parentPos_ = {0, 0};
    color_ = "white";
    dis_ = numeric_limits<double>::infinity();
    hasParent_ = false;
}

BFS_Vertex::BFS_Vertex(pair<int, int> coords) {
    vertexPos_ = coords;
    parentPos_ = coords;
    color_ = "white";
    dis_ = numeric_limits<double>::infinity();
    hasParent_ = false;
}

BFS_Vertex::BFS_Vertex(pair<int, int> vertex_pos, pair<int, int> parent_pos,
    string color, double dis, bool has_parent) {
        vertexPos_ = vertex_pos;
        parentPos_ = parent_pos;
        color_ = color;
        dis_ = dis;
        hasParent_ = has_parent;
    }

BFS_Vertex::BFS_Vertex(const BFS_Vertex& bfs_vertex) {
    vertexPos_ = bfs_vertex.getVertexPos();
    parentPos_ = bfs_vertex.getParentPos();
    color_ = bfs_vertex.getColor();
    dis_ = bfs_vertex.getDis();
    hasParent_ = bfs_vertex.hasParent();
}

BFS_Vertex::BFS_Vertex(BFS_Vertex&& bfs_vertex) noexcept {
    vertexPos_ = bfs_vertex.getVertexPos();
    parentPos_ = bfs_vertex.getParentPos();
    color_ = bfs_vertex.getColor();
    dis_ = bfs_vertex.getDis();
    hasParent_ = bfs_vertex.hasParent();
    bfs_vertex.resetBFSVertex();
}

/* -------------------------------------------- OPERATORS: ------------------------------------------- */

BFS_Vertex& BFS_Vertex::operator=(const BFS_Vertex& bfs_vertex) {
    if (this != &bfs_vertex) {
        vertexPos_ = bfs_vertex.getVertexPos();
        parentPos_ = bfs_vertex.getParentPos();
        color_ = bfs_vertex.getColor();
        dis_ = bfs_vertex.getDis();
        hasParent_ = bfs_vertex.hasParent();
    }
    return *this;
}

BFS_Vertex& BFS_Vertex::operator=(BFS_Vertex&& bfs_vertex) noexcept {
    if (this != &bfs_vertex) {
        vertexPos_ = bfs_vertex.getVertexPos();
        parentPos_ = bfs_vertex.getParentPos();
        color_ = bfs_vertex.getColor();
        dis_ = bfs_vertex.getDis();
        hasParent_ = bfs_vertex.hasParent();
        bfs_vertex.resetBFSVertex();
    }
    return *this;
}

bool BFS_Vertex::operator==(const BFS_Vertex& bfs_vertex) const {
    return (vertexPos_ == bfs_vertex.getVertexPos() && parentPos_ == bfs_vertex.getParentPos() &&
            color_ == bfs_vertex.getColor() && dis_ == bfs_vertex.getDis() &&
            hasParent_ == bfs_vertex.hasParent());
}

bool BFS_Vertex::operator!=(const BFS_Vertex& bfs_vertex) const {
    return !(*this == bfs_vertex);
}

/* --------------------------------------------- GETTERS: -------------------------------------------- */

pair<int, int> BFS_Vertex::getVertexPos() const noexcept {
    return vertexPos_;
}

pair<int, int> BFS_Vertex::getParentPos() const noexcept {
    return parentPos_;
}

string BFS_Vertex::getColor() const {
    return color_;
}

double BFS_Vertex::getDis() const {
    return dis_;
}

bool BFS_Vertex::hasParent() const noexcept {
    return hasParent_;
}

/* --------------------------------------------- SETTERS: -------------------------------------------- */

void BFS_Vertex::setVertexPos(pair<int, int> pos) {
    if (pos.first < 0) {
        throw invalid_argument("Cannot set a vertex's coordinates with a negative X coordinate!");
    }
    else if (pos.second < 0) {
        throw invalid_argument("Cannot set a vertex's coordinates with a negative Y coordinate!");
    }
    vertexPos_ = pos;
}

void BFS_Vertex::setParentPos(pair<int, int> pos) {
    if (pos.first < 0) {
        throw invalid_argument("Cannot set a parent vertex's coordinates with a negative X coordinate!");
    }
    else if (pos.second < 0) {
        throw invalid_argument("Cannot set a parent vertex's coordinates with a negative Y coordinate!");
    }
    parentPos_ = pos;
}

void BFS_Vertex::setColor(const std::string& color) {
    if (!isValidColor(color)) {
        throw invalid_argument("Cannot set a BFS vertex's color to any color besides white, grey, or black!");
    }
    color_ = color;
}

void BFS_Vertex::setDis(double dis) {
    if (dis < 0) {
        throw invalid_argument("Cannot set a BFS vertex's distance from tankA's position upon starting the BFS to a negative number!");
    }
    dis_ = dis;
}

void BFS_Vertex::setHasParent(bool has_parent) {
    hasParent_ = has_parent;
}

/* ----------------------------------------- OTHER FUNCTIONS: ---------------------------------------- */

bool BFS_Vertex::isValidColor(const std::string& color) {
    return (color == "white" || color == "gray" || color == "black");
}

void BFS_Vertex::resetBFSVertex() {
    vertexPos_ = {0, 0};
    parentPos_ = {0, 0};
    color_ = "white";
    dis_ = 0;
    hasParent_ = false;
}