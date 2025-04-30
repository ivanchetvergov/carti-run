#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "../include/player.h"
#include "../include/level.h"

#include <iostream>
#include <vector>
#include <optional>
#include <variant>  // Нужен для std::holds_alternative


int main() {
    // Создаём окно с разрешением 1024×768
    sf::RenderWindow window(sf::VideoMode({1024, 768}), "Carti run");

    // Создаём игрока. Убедитесь, что у класса Player есть метод getPosition(),
    // возвращающий sf::Vector2f с позицией игрока.
    Player player("../assets/ful_anim.png");

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

    auto spikesTexture = std::make_shared<sf::Texture>();
    if (!spikesTexture->loadFromFile("../assets/spikes.png")){
        throw std::runtime_error("Failed to load spikes texture");
    }
    level.setSpikeTexture(spikesTexture);

    std::vector<std::string> levelData = {
        "_____1111111_____________________________",
        "1111110000011101_________________________",
        "1000000000000001_________________________",
        "1000111100000001______11_________________",
        "1000000000110001_________________________",
        "1110000000000011______________11_________",
        "1000000000000001_________________________",
        "1000111000110001______11_________________",
        "1000000000000001_________________________",
        "1001100000000001___!!!!__________________",
        "10000000000000011__1111__1___111__11__11_",
        "100000000000000_________________________1",
        "111111100000000__________________________",
        "111111111110000__________________________",
        "111111111111111___1__1__1__1__1___1____11",
        "______________!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    };
/*
    std::vector<std::string> levelData = {
        "__________________________________________",
        "_____0000000__________0000000_____________",
        "_____0111110__________0111110_____________",
        "__________________________________________",
        "___111_________0000_________111___________",
        "___100_________0110_________001___________",
        "___100_______________________001__________",
        "___100______!!!!_____!!!!____001__________",
        "___100______0000_____0000____001__________",
        "___1111111_________________11111__________",
        "_________1_________________1______________",
        "_________1_______0000______1______________",
        "_________1_______0!!0______1______________",
        "_________11111111000!!!!!!!!!!!!!!!_______",
        "__________________________________________",
        "___________________111111111111111111111__"
    };
*/

    level.loadLevel(levelData);

    sf::Music backgroundMusic;
    if (!backgroundMusic.openFromFile("../assets/differentDay.mp3")) {
        throw std::runtime_error("Failed to load music");
    }
    backgroundMusic.setVolume(1.f);
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

    sf::Font font;
    if (!font.openFromFile("../assets/menu_font.ttf")) {
        throw std::runtime_error("failed to load font");
    }
    
    sf::Text deathText(font, "YOU'RE DEAD!\npress [R] to respawn\npress [Q] to quit", 40);
    deathText.setFillColor(sf::Color::Red);
    deathText.setPosition(sf::Vector2f(200.f, 200.f));
    

    bool showDeathScreen = false;

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        std::optional<sf::Event> eventOpt;
        while ((eventOpt = window.pollEvent())) {
            const sf::Event& event = *eventOpt;

            if (event.is<sf::Event::Closed>()) {
                window.close();
            }

            if (showDeathScreen) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)){
                    player.respawn();
                    showDeathScreen = false;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)){
                    window.close();
                }
                continue;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::K)){
            player.kill();
        }

        if (player.get_isDead()) {
            showDeathScreen = true;
        }

        if (!showDeathScreen) {
            player.update(deltaTime, level);
        }

        sf::Vector2f playerPos = player.getPosition();
        camera.setCenter(playerPos);
        window.setView(camera);

        window.clear();

        // фон
        window.setView(window.getDefaultView());
        window.draw(cloudsSprite);

        // мир и игрок
        window.setView(camera);
        level.draw(window);
        player.draw(window);

        if (showDeathScreen) {
            window.setView(window.getDefaultView());
            window.draw(deathText);
        }

        window.display();
    }

    return 0;
}







