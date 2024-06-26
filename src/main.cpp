#include "simulation.hpp"

#include <SFML/Graphics.hpp>
#include <iostream>

constexpr uint WIDTH = 1280;
constexpr uint HEIGHT = 960;
constexpr uint CELL_SIZE = 20;

void help(std::string program) {
    std::cout
        << "Usage: " << program << " [FILE]\n\n"
        << "[Control]\n"
        << "WASD     camera control\n"
        << "+-       camera zoom\n"
        << "Space    pause\n"
        << "P        get pattern\n"
        << "O        return to the initial state\n";
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

void editorMode(Simulation& simulation, sf::RenderWindow& window) {
    bool leftButtonPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    bool rightButtonPressed = sf::Mouse::isButtonPressed(sf::Mouse::Right);

    if (leftButtonPressed || rightButtonPressed) {
        auto pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        simulation.setCell(pos, leftButtonPressed && !rightButtonPressed);
    }
}

int main(int argc, char* argv[]) {
    try {
        if (argc == 1) {
            throw std::runtime_error("Failed to get configuration");
        }

        if (std::string(argv[1]) == "--help") {
            help(argv[0]);
            return 0;
        }

        Config config(argv[1]);
        bool stop = true;
        uint time = 0;

        auto gridSize = config.getGridSize();
        auto rule = config.getRule();
        auto state = config.getInitialState();
        auto delay = sf::milliseconds(config.getDelay());

        sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Cellular automata");
        window.setVerticalSyncEnabled(true);

        sf::View view(window.getDefaultView());
        view.setCenter(
            (gridSize.cols / 2) * CELL_SIZE,
            (gridSize.rows / 2) * CELL_SIZE
        );
        window.setView(view);

        Simulation simulation(gridSize, rule, state, CELL_SIZE);
        sf::Clock clock;

        Pattern pattern = config.convertToPattern(state);

        while (window.isOpen()) {
            sf::Event event;

            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                } else if (event.type == sf::Event::Resized) {
                    view.setSize(event.size.width, event.size.height);
                    window.setView(view);
                } else if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Space) {
                        stop = !stop;
                    }

                    if (stop && event.key.code == sf::Keyboard::P) {
                        auto& state = simulation.getState();
                        pattern = config.convertToPattern(state);
                    }

                    if (stop && event.key.code == sf::Keyboard::O) {
                        simulation.setState(state);
                        time = 0;
                    }
                }
            }

            system("clear");
            std::cout
                << "[time]\n" << time << "\n"
                << pattern;

            if (changeView(view)) {
                window.setView(view);
            }

            if (stop) {
                editorMode(simulation, window);
            }

            if (!stop && clock.getElapsedTime() > delay) {
                simulation.nextState();
                clock.restart();
                time += 1;
            }

            window.clear(sf::Color::White);
            simulation.drawGrid(window);
            simulation.drawCells(window);
            window.display();
        }
    } catch(const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}
