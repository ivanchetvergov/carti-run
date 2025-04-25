#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "player.h"
#include "level.h"
#include <iostream>
#include <vector>

int main() {
    sf::RenderWindow window(sf::VideoMode({1024, 768}), "Carti run");

    Player player("../assets/catri64.png");

    // Создаём уровень с пустым конструктором
    Level level;

    // Устанавливаем текстуры и размеры через методы
    auto wallTexture = std::make_shared<sf::Texture>();
    if (!wallTexture->loadFromFile("../assets/block.png")) {
        throw std::runtime_error("Failed to load wall texture");
    }
    level.setWallTexture(wallTexture);

    auto backgroundTexture = std::make_shared<sf::Texture>();
    if (!backgroundTexture->loadFromFile("../assets/background.png")) {
        throw std::runtime_error("Failed to load background texture");
    }
    level.setBackgroundTexture(backgroundTexture);

    std::vector<std::string> levelData = {
        "1111111111111111",
        "1000000000000001",
        "1000111100110001",
        "1000000000000001",
        "1110000000001101",
        "1000000000000001",
        "1000111000110001",
        "1000000000000001",
        "1001100000000001",
        "1000000000000001",
        "1000000000000001",
        "1111111111111111"
    };
    

    level.loadLevel(levelData); // Загружаем данные уровня

    sf::Music backgroundMusic;
    if (!backgroundMusic.openFromFile("../assets/differentDay.mp3")) {
        throw std::runtime_error("Failed to load  music");
    }
    
    
    backgroundMusic.setVolume(50.f); // Устанавливаем громкость (0-100)
    backgroundMusic.play();          // Запускаем музыку

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
