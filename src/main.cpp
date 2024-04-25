#include "config.hpp"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

constexpr uint WIDTH = 1280;
constexpr uint HEIGHT = 960;
constexpr uint CELL_SIZE = 50;

sf::VertexArray buildGrid(uint rows, uint cols, uint cellSize) {
    uint vertexCount = (rows + cols + 2) * 2;
    sf::VertexArray grid(sf::Lines, vertexCount);

    uint idx = 0;

    for (uint i = 0; i <= rows; ++i) {
        grid[idx].position = sf::Vector2f(0, i * cellSize);
        grid[idx].color = sf::Color(80, 80, 80);

        grid[idx + 1].position = sf::Vector2f(cols * cellSize, i * cellSize);
        grid[idx + 1].color = sf::Color(80, 80, 80);
        idx += 2;
    }

    for (uint i = 0; i <= cols; ++i) {
        grid[idx].position = sf::Vector2f(i * cellSize, 0);
        grid[idx].color = sf::Color(80, 80, 80);

        grid[idx + 1].position = sf::Vector2f(i * cellSize, rows * cellSize);
        grid[idx + 1].color = sf::Color(80, 80, 80);
        idx += 2;
    }

    return grid;
}

void buildCells(const std::vector<bool>& state, std::vector<sf::RectangleShape>& cells, uint rows, uint cols) {
    for (uint i = 0; i < rows; ++i) {
        for (uint j = 0; j < cols; ++j) {
            if (!state[(cols * i) + j]) {
                continue;
            }

            cells[(cols * i) + j].setPosition(sf::Vector2f(j * CELL_SIZE, i * CELL_SIZE));
            cells[(cols * i) + j].setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
            cells[(cols * i) + j].setFillColor(sf::Color::White);
        }
    }
}

bool changeView(sf::View& view) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Dash)) {
        view.setSize(view.getSize() * 1.02f);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Equal)) {
        view.setSize(view.getSize() / 1.02f);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        view.move(0.f, -20.f);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        view.move(-20.f, 0.f);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        view.move(0.f, 20.f);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        view.move(20.f, 0.f);
    } else {
        return false;
    }

    return true;
}

int main(int argc, char* argv[]) {
    try {
        Config config(argv[1]);
        auto gridSize = config.getGridSize();
        auto rule = config.getRule();

        sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Cellular automata");
        window.setVerticalSyncEnabled(true);

        sf::View view(window.getDefaultView());
        auto grid = buildGrid(gridSize.rows, gridSize.cols, CELL_SIZE);
        auto state = config.getInitialState();

        std::vector<sf::RectangleShape> cells(gridSize.rows * gridSize.cols);
        buildCells(state, cells, gridSize.rows, gridSize.cols);

        while (window.isOpen()) {
            sf::Event event;

            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                } else if (event.type == sf::Event::Resized) {
                    view.reset(sf::FloatRect(0.f, 0.f, event.size.width, event.size.height));
                    window.setView(view);
                }
            }

            if (changeView(view)) {
                window.setView(view);
            }

            window.clear();
            window.draw(grid);

            for (uint i = 0; i < cells.size(); ++i) {
                if (state[i]) {
                    window.draw(cells[i]);
                }
            }

            window.display();
        }
    }
    catch(const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}
