#include "BaseSatelliteView.h"

const std::vector<std::vector<char>>& BaseSatelliteView::getGrid() const {
    return grid_;
}

void BaseSatelliteView::setGrid(const std::vector<std::vector<char>>& grid) {
    grid_ = grid;
}

char BaseSatelliteView::getObjectAt(size_t x, size_t y) const {
    if (y >= grid_.size() || x >= grid_[y].size()) {
        return '&'; // Out of bounds
    }
    
    if (currTankInd_ >= 0 && static_cast<size_t>(currTankInd_) < positions_.size()) {
        const auto& currPos = positions_[currTankInd_];
        if (currPos.first == y && currPos.second == x) {
            return '%'; // It's the current tank's position
        }
    }

    return grid_[y][x]; // grid is row-major: grid_[y][x]
}

const std::vector<std::pair<size_t, size_t>>& BaseSatelliteView::getPositions() const {
    return positions_;
}

void BaseSatelliteView::setPositions(const std::vector<std::pair<size_t, size_t>>& positions) {
    positions_ = positions;
}

void BaseSatelliteView::addPosition(size_t x, size_t y) {
    positions_.emplace_back(x, y);
}

void BaseSatelliteView::setPosition(size_t index, size_t x, size_t y) {
    if (index < positions_.size()) {
        positions_[index] = {x, y};
    }
}

void BaseSatelliteView::setCurrTankInd(int ind) {
    currTankInd_ = ind;
}

void BaseSatelliteView::updateGrid(const std::vector<std::vector<std::string>>& newGrid) {
    for (size_t y = 0; y < newGrid.size(); ++y) {
        for (size_t x = 0; x < newGrid[y].size(); ++x) {
            if (newGrid[y][x] == "."){
                grid_[y][x] = ' ';
            }
            else if (!newGrid[y][x].empty()) {
                grid_[y][x] = newGrid[y][x][0];
            } else {
                grid_[y][x] = ' ';
            }
        }
    }
}

void BaseSatelliteView::initializeGrid(size_t height, size_t width) {
    grid_ = std::vector<std::vector<char>>(height, std::vector<char>(width, ' '));
}