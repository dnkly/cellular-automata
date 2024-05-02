#include "simulation.hpp"

std::vector<sf::Vertex> Simulation::buildGrid() {
    uint rows = gridSize_.rows;
    uint cols = gridSize_.cols;
    uint vertexCount = (rows + cols + 2) * 2;

    std::vector<sf::Vertex> grid(vertexCount);
    sf::Color color(80, 80, 80);

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

void Simulation::drawGrid(sf::RenderWindow& window) {
    window.draw(&grid_[0], grid_.size(), sf::Lines);
}

void Simulation::drawCells(sf::RenderWindow& window) {
    buildCells();

    for (uint i = 0; i < state_.size(); ++i) {
        if (state_[i]) {
            window.draw(cells_[i]);
        }
    }
}
