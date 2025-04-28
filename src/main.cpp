#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "player.h"
#include "level.h"
#include <iostream>
#include <vector>
#include <optional>
#include <variant>  // Нужен для std::holds_alternative

int main() {
    // Создаём окно с разрешением 1024×768
    sf::RenderWindow window(sf::VideoMode({1024, 768}), "Carti run");

    // Создаём игрока. Убедитесь, что у класса Player есть метод getPosition(),
    // возвращающий sf::Vector2f с позицией игрока.
    Player player("../assets/mario_run.png");

    // Создание и настройка уровня
    Level level;
    auto wallTexture = std::make_shared<sf::Texture>();
    if (!wallTexture->loadFromFile("../assets/block.png")) {
        throw std::runtime_error("Failed to load wall texture");
    }
    level.setWallTexture(wallTexture);

    auto emptyTexture = std::make_shared<sf::Texture>();
    if (!emptyTexture->loadFromFile("../assets/empty.png")) {
        throw std::runtime_error("Failed to load empty texture");
    }
    level.setEmptyTexture(emptyTexture);

    auto backgroundTexture = std::make_shared<sf::Texture>();
    if (!backgroundTexture->loadFromFile("../assets/background.png")) {
        throw std::runtime_error("Failed to load background texture");
    }
    level.setBackgroundTexture(backgroundTexture);

    auto cloudsTexture = std::make_shared<sf::Texture>();
    if (!cloudsTexture->loadFromFile("../assets/clouds.png")){
        throw std::runtime_error("Failed to load clouds texture");
    }

    level.setCloudTexture(cloudsTexture);

    std::vector<std::string> levelData = {
        "_____1111111____",
        "1111110000011111",
        "1000000000000001",
        "1000111100000001",
        "1000000000110001",
        "1110000000000011",
        "1000000000000001",
        "1000111000110001",
        "1000000000000001",
        "1001100000000001",
        "1000000000000001",
        "1000000000000001",
        "1111111111111001",
        "1111111111111001",
        "1111111111111111"
    };

    level.loadLevel(levelData);

    sf::Music backgroundMusic;
    if (!backgroundMusic.openFromFile("../assets/differentDay.mp3")) {
        throw std::runtime_error("Failed to load music");
    }
    backgroundMusic.setVolume(0.80f);
    backgroundMusic.play();

    // Создаём камеру (sf::View). Конструктор sf::FloatRect в SFML 3 принимает два вектора:
    // позицию и размер.
    sf::View camera(sf::FloatRect(sf::Vector2f(0.f, 0.f), sf::Vector2f(1024.f, 768.f)));

    // Создаем спрайт для облаков. Указываем текстуру в конструкторе.
    sf::Sprite cloudsSprite(*cloudsTexture);

    // Масштабируем спрайт так, чтобы он заполнил окно
    sf::Vector2u windowSize = window.getSize();
    float scaleX = static_cast<float>(windowSize.x) / cloudsTexture->getSize().x;
    float scaleY = static_cast<float>(windowSize.y) / cloudsTexture->getSize().y;
    cloudsSprite.setScale(sf::Vector2f(scaleX, scaleY)); // Передаем sf::Vector2f

    sf::Clock clock;

    while (window.isOpen()) {
        std::optional<sf::Event> eventOpt;
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        float deltaTime = clock.restart().asSeconds();
        player.update(deltaTime, level);

        // Устанавливаем позицию камеры в соответствии с позицией игрока
        sf::Vector2f playerPos = player.getPosition();
        camera.setCenter(playerPos);
        window.setView(camera);

        // 1. Очистка окна
        window.clear();  // Теперь удаляем чёрный фон (без sf::Color::Black)

        // 2. Отрисовка фона с облаками
        window.setView(window.getDefaultView());  // Устанавливаем статический вид
        window.draw(cloudsSprite);               // Отрисовываем фон с облаками

        // 3. Отрисовка уровня и игрока
        window.setView(camera);                  // Переключаемся на камеру
        level.draw(window);
        player.draw(window);

        // 4. Обновляем окно
        window.display();
    }        

    return 0;
}
