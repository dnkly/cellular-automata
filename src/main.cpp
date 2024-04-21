#include <SFML/Graphics.hpp>

constexpr int WIDTH = 1024;
constexpr int HEIGHT = 768;

sf::VertexArray buildGrid(uint rows, uint cols, uint cellSize) {
    uint vertexCount = (rows + cols + 2) * 2;
    sf::VertexArray grid(sf::Lines, vertexCount);

    uint idx = 0;

    for (uint i = 0; i <= rows; ++i) {
        grid[idx].position = sf::Vector2f(0, i * cellSize);
        grid[idx + 1].position = sf::Vector2f(cols * cellSize, i * cellSize);
        idx += 2;
    }

    for (uint i = 0; i <= cols; ++i) {
        grid[idx].position = sf::Vector2f(i * cellSize, 0);
        grid[idx + 1].position = sf::Vector2f(i * cellSize, rows * cellSize);
        idx += 2;
    }

    return grid;
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

int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Cellular automata");
    window.setVerticalSyncEnabled(true);

    sf::View view(window.getDefaultView());
    auto grid = buildGrid(30, 50, 50);

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
        window.display();
    }

    return 0;
}
