#pragma once

#include "SatelliteView.h"
#include <vector>
#include <utility>

class BaseSatelliteView : public SatelliteView {
private:
    std::vector<std::vector<char>> grid_;
    std::vector<std::pair<size_t, size_t>> positions_;
    int currTankInd_;

public:
    virtual ~BaseSatelliteView() override = default;

    const std::vector<std::vector<char>>& getGrid() const;

    void setGrid(const std::vector<std::vector<char>>& grid);

    char getObjectAt(size_t x, size_t y) const override;

    const std::vector<std::pair<size_t, size_t>>& getPositions() const;

    void setPositions(const std::vector<std::pair<size_t, size_t>>& positions);

    void addPosition(size_t x, size_t y);

    void setPosition(size_t index, size_t x, size_t y);

    void setCurrTankInd(int ind);

    void updateGrid(const std::vector<std::vector<std::string>>& newGrid);

    void initializeGrid(size_t height, size_t width);
};