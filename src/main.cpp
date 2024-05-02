#include "simulation.hpp"

#include <SFML/Graphics.hpp>
#include <iostream>

constexpr uint WIDTH = 1280;
constexpr uint HEIGHT = 960;
constexpr uint CELL_SIZE = 50;

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
        if (argc == 1) {
            throw std::runtime_error("Failed to get configuration");
        }

        Config config(argv[1]);
        bool stop = false;

        auto gridSize = config.getGridSize();
        auto rule = config.getRule();
        auto state = config.getInitialState();
        auto delay = sf::milliseconds(config.getDelay());

        sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Cellular automata");
        window.setVerticalSyncEnabled(true);

        sf::View view(window.getDefaultView());
        sf::Clock clock;

        Simulation simulation(gridSize, rule, state, CELL_SIZE);
        simulation.buildCells();

        while (window.isOpen()) {
            sf::Event event;

            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                } else if (event.type == sf::Event::Resized) {
                    view.reset(sf::FloatRect(0.f, 0.f, event.size.width, event.size.height));
                    window.setView(view);
                } else if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Space) {
                        stop = !stop;
                    }
                }
            }

            if (changeView(view)) {
                window.setView(view);
            }

            if (!stop && clock.getElapsedTime() > delay) {
                simulation.nextState();
                simulation.buildCells();
                clock.restart();
            }

            window.clear();
            simulation.drawGrid(window);
            simulation.drawCells(window);
            window.display();
        }
    } catch(const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}
