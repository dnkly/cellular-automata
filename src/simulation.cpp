#include "simulation.hpp"

std::vector<sf::Vertex> Simulation::buildGrid() {
    uint vertexCount = (rows_ + cols_ + 2) * 2;

    std::vector<sf::Vertex> grid(vertexCount);
    sf::Color color(200, 200, 200);

    uint k = 0;

    for (uint i = 0; i <= rows_; ++i) {
        grid[k].position = sf::Vector2f(0, i * cellSize_);
        grid[k].color = color;

        grid[k + 1].position = sf::Vector2f(cols_ * cellSize_, i * cellSize_);
        grid[k + 1].color = color;
        k += 2;
    }

    for (uint i = 0; i <= cols_; ++i) {
        grid[k].position = sf::Vector2f(i * cellSize_, 0);
        grid[k].color = color;

        grid[k + 1].position = sf::Vector2f(i * cellSize_, rows_ * cellSize_);
        grid[k + 1].color = color;
        k += 2;
    }

    return grid;
}

std::vector<sf::RectangleShape> Simulation::buildCells() {
    std::vector<sf::RectangleShape> cells(state_.size());

    for (uint i = 0; i < rows_; ++i) {
        for (uint j = 0; j < cols_; ++j) {
            cells[idx(i, j)].setPosition(j * cellSize_, i * cellSize_);
            cells[idx(i, j)].setSize(sf::Vector2f(cellSize_, cellSize_));
            cells[idx(i, j)].setFillColor(sf::Color::Black);
        }
    }

    return cells;
}

uint Simulation::countNeighbors(uint i, uint j) {
    uint count = 0;

    for (uint p = i - 1; p <= i + 1; ++p) {
        for (uint q = j - 1; q <= j + 1; ++q) {
            if (p == i && q == j) {
                continue;
            }

            if (state_[idx(p, q)]) {
                ++count;
            }
        }
    }

    return count;
}

void Simulation::nextState() {
    auto& B = rule_.B;
    auto& S = rule_.S;
    std::vector<bool> newState(state_.size());

    for (uint i = 1; i < rows_ - 1; ++i) {
        for (uint j = 1; j < cols_ - 1; ++j) {
            uint count = countNeighbors(i, j);

            newState[idx(i, j)] = state_[idx(i, j)]
                ? std::find(S.cbegin(), S.cend(), count) != S.cend()
                : std::find(B.cbegin(), B.cend(), count) != B.cend();
        }
    }

    state_ = std::move(newState);
}

void Simulation::drawGrid(sf::RenderWindow& window) {
    window.draw(&grid_[0], grid_.size(), sf::Lines);
}

const std::vector<bool>& Simulation::getState() {
    return state_;
}

void Simulation::drawCells(sf::RenderWindow& window) {
    for (uint i = 0; i < state_.size(); ++i) {
        if (state_[i]) {
            window.draw(cells_[i]);
        }
    }
}

void Simulation::setCell(const sf::Vector2f& pos, bool value) {
    for (uint i = 0; i < state_.size(); ++i) {
        auto diff = pos - cells_[i].getPosition();

        bool flag = diff.x > 0 && diff.x < cellSize_
            && diff.y > 0 && diff.y < cellSize_;

        if (flag) {
            state_[i] = value;
            break;
        }
    }
}

void Simulation::setState(const std::vector<bool>& state) {
    state_.assign(state.cbegin(), state.cend());
}
