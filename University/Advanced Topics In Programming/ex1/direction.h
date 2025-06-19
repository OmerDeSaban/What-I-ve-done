/**
 * @file direction.h
 * @brief A file depicting the possible directions each element could have on the board.
 * The maximal number of columns in a comment-line in this file is 106.
 */

#pragma once
#include <string>
#include <stdexcept>
#include <utility>
using namespace std;

enum class Direction {
    U,   // Up
    UR,  // Up-Right
    R,   // Right
    DR,  // Down-Right
    D,   // Down
    DL,  // Down-Left
    L,   // Left
    UL   // Up-Left
};

// String conversion helpers
inline std::string directionToString(Direction dir) {
    switch (dir) {
        case Direction::U:  return "U";
        case Direction::UR: return "UR";
        case Direction::R:  return "R";
        case Direction::DR: return "DR";
        case Direction::D:  return "D";
        case Direction::DL: return "DL";
        case Direction::L:  return "L";
        case Direction::UL: return "UL";
        default: throw std::invalid_argument("Unknown direction");
    }
}

inline Direction stringToDirection(const std::string& str) {
    if (str == "U")  return Direction::U;
    if (str == "UR") return Direction::UR;
    if (str == "R")  return Direction::R;
    if (str == "DR") return Direction::DR;
    if (str == "D")  return Direction::D;
    if (str == "DL") return Direction::DL;
    if (str == "L")  return Direction::L;
    if (str == "UL") return Direction::UL;
    throw std::invalid_argument("Invalid direction string: " + str);
}

inline int directionToIndex(Direction dir) {
    switch (dir) {
        case Direction::U:  return 0;
        case Direction::UR: return 1;
        case Direction::R:  return 2;
        case Direction::DR: return 3;
        case Direction::D:  return 4;
        case Direction::DL: return 5;
        case Direction::L:  return 6;
        case Direction::UL: return 7;
        default: throw std::invalid_argument("Unknown direction");
    }
}

inline Direction indexToDirection(int index) {
    if (index == 0)  return Direction::U;
    if (index == 1) return Direction::UR;
    if (index == 2)  return Direction::R;
    if (index == 3) return Direction::DR;
    if (index == 4)  return Direction::D;
    if (index == 5) return Direction::DL;
    if (index == 6)  return Direction::L;
    if (index == 7) return Direction::UL;
    throw std::invalid_argument("Invalid index for direction");
}

inline std::pair<int, int> directionToDelta(Direction dir) {
    switch (dir) {
        case Direction::U:  return {-1, 0};
        case Direction::UR: return {-1, 1};
        case Direction::R:  return {0, 1};
        case Direction::DR: return {1, 1};
        case Direction::D:  return {1, 0};
        case Direction::DL: return {1, -1};
        case Direction::L:  return {0, -1};
        case Direction::UL: return {-1, -1};
        default: throw std::invalid_argument("Unknown direction");
    }
}

inline Direction rotateRightQuarter(Direction dir) {
    int index = directionToIndex(dir);
    return indexToDirection((index + 2) % 8);
}

inline Direction rotateLeftQuarter(Direction dir) {
    int index = directionToIndex(dir);
    return indexToDirection((index + 6) % 8);  // Equivalent to -2 mod 8
}

inline Direction rotateRightEighth(Direction dir) {
    int index = directionToIndex(dir);
    return indexToDirection((index + 1) % 8);
}

inline Direction rotateLeftEighth(Direction dir) {
    int index = directionToIndex(dir);
    return indexToDirection((index + 7) % 8);  // Equivalent to -1 mod 8
}

inline pair<int, int> getNextCoordsInDir(Direction dir, pair<int, int> prevCoords, int rows, int cols) {
    pair<int, int> delta = directionToDelta(dir);
    int newRow = (prevCoords.first + delta.first + rows) % rows;
    int newCol = (prevCoords.second + delta.second + cols) % cols;
    return {newRow, newCol};
}