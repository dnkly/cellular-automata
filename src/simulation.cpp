#include "simulation.hpp"

std::vector<sf::Vertex> Simulation::buildGrid() {
    uint rows = gridSize_.rows;
    uint cols = gridSize_.cols;
    uint vertexCount = (rows + cols + 2) * 2;

    std::vector<sf::Vertex> grid(vertexCount);
    sf::Color color(20, 20, 20);

    uint k = 0;

    for (uint i = 0; i <= rows; ++i) {
        grid[k].position = sf::Vector2f(0, i * cellSize_);
        grid[k].color = color;

        grid[k + 1].position = sf::Vector2f(cols * cellSize_, i * cellSize_);
        grid[k + 1].color = color;
        k += 2;
    }

    for (uint i = 0; i <= cols; ++i) {
        grid[k].position = sf::Vector2f(i * cellSize_, 0);
        grid[k].color = color;

        grid[k + 1].position = sf::Vector2f(i * cellSize_, rows * cellSize_);
        grid[k + 1].color = color;
        k += 2;
    }

    return grid;
}

void Simulation::buildCells() {
    uint rows = gridSize_.rows;
    uint cols = gridSize_.cols;

    for (uint i = 0; i < rows; ++i) {
        for (uint j = 0; j < cols; ++j) {
            uint k = (cols * i) + j;

            if (!state_[k]) {
                continue;
            }

            cells_[k].setPosition(j * cellSize_, i * cellSize_);
            cells_[k].setSize(sf::Vector2f(cellSize_, cellSize_));
        }
    }
}

uint Simulation::countNeighbors(uint idx, uint jdx) {
    uint cols = gridSize_.cols;
    uint count = 0;

    for (uint i = idx - 1; i <= idx + 1; ++i) {
        for (uint j = jdx - 1; j <= jdx + 1; ++j) {
            if (i == idx && j == jdx) {
                continue;
            }

            if (state_[(cols * i) + j]) {
                ++count;
            }
        }
    }

    return count;
}

void Simulation::nextState() {
    uint rows = gridSize_.rows;
    uint cols = gridSize_.cols;

    auto& B = rule_.B;
    auto& S = rule_.S;
    std::vector<bool> newState(state_.size());

    for (uint i = 1; i < rows - 1; ++i) {
        for (uint j = 1; j < cols - 1; ++j) {
            uint count = countNeighbors(i, j);
            uint k = (cols * i) + j;

            newState[k] = state_[k]
                ? std::find(S.cbegin(), S.cend(), count) != S.cend()
                : std::find(B.cbegin(), B.cend(), count) != B.cend();
        }
    }

    state_ = std::move(newState);
}

void Simulation::drawGrid(sf::RenderWindow& window) {
    window.draw(&grid_[0], grid_.size(), sf::Lines);
}

void Simulation::drawCells(sf::RenderWindow& window) {
    for (uint i = 0; i < state_.size(); ++i) {
        if (state_[i]) {
            window.draw(cells_[i]);
        }
    }
}
