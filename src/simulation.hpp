#pragma once

#include "config.hpp"

#include <SFML/Graphics.hpp>
#include <vector>

class Simulation {
private:
    const GridSize gridSize_;
    const uint cellSize_;
    std::vector<sf::Vertex> grid_;

    const Rule rule_;
    std::vector<bool> state_;
    std::vector<sf::RectangleShape> cells_;

    std::vector<sf::Vertex> buildGrid();
    uint countNeighbors(uint i, uint j);
public:
    Simulation(
        const GridSize& gridSize,
        const Rule& rule,
        const std::vector<bool>& state,
        uint cellSize
    ) : gridSize_(gridSize), cellSize_(cellSize), rule_(rule), state_(state) {
        grid_ = buildGrid();
        cells_ = std::vector<sf::RectangleShape>(state.size());
    }

    void drawGrid(sf::RenderWindow& window);
    void drawCells(sf::RenderWindow& window);

    void buildCells();
    void nextState();
};
