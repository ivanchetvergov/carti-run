#include <SFML/Graphics.hpp>
#include "player.h"
#include "level.h"
#include <iostream>
#include <vector>

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Carti run");

    Player player("../assets/carti.png");
    Level level("../assets/block.png", 10, 10);

    std::vector<std::string> levelData = {
        "1111111111",
        "1000000001",
        "1001110001",
        "1000000001",
        "1011001101",
        "1000000001",
        "1111111111",
        "1000000001",
        "1000000001",
        "1111111111"
    };

    level.loadLevel(levelData);

    sf::Clock clock;

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        float deltaTime = clock.restart().asSeconds();

        player.handleInput(deltaTime, level);

        window.clear(sf::Color::Black);
        level.draw(window);
        player.draw(window);
        window.display();
    }

    return 0;
}
