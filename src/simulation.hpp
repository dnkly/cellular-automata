#pragma once

#include "config.hpp"

#include <SFML/Graphics.hpp>
#include <vector>

class Simulation {
private:
    const uint rows_;
    const uint cols_;
    const uint cellSize_;
    std::vector<sf::Vertex> grid_;

    const Rule rule_;
    std::vector<bool> state_;
    std::vector<sf::RectangleShape> cells_;

    std::vector<sf::Vertex> buildGrid();
    std::vector<sf::RectangleShape> buildCells();
    uint countNeighbors(uint i, uint j);

    inline uint idx(uint i, uint j) {
        return (cols_ * i) + j;
    }
public:
    Simulation(
        const GridSize& gridSize,
        const Rule& rule,
        const std::vector<bool>& state,
        uint cellSize
    ) :
        rows_(gridSize.rows),
        cols_(gridSize.cols),
        cellSize_(cellSize),
        rule_(rule),
        state_(state)
    {
        grid_ = buildGrid();
        cells_ = buildCells();
    }

    void drawGrid(sf::RenderWindow& window);
    void drawCells(sf::RenderWindow& window);
    const std::vector<bool>& getState();

    void setCell(const sf::Vector2f& pos, bool value);
    void nextState();
};
