#include "../include/game.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../include/player.h"
#include "../include/level.h"
#include <iostream>
#include <vector>
#include <optional>
#include <memory>
#include <string>
#include <sstream>
#include <iomanip>


// Конструктор Game: все поля, у которых нет дефолтных конструкторов, инициализируются через список инициализации.
Game::Game()
  : deathWindowSprite(deathWindowTexture),
    winWindowSprite(winWindowTexture),
    cloudsSprite(deathWindowTexture),
    window(sf::VideoMode({1024, 768}), "Carti run"),
    camera(sf::FloatRect(sf::Vector2f(0.f, 0.f), sf::Vector2f(1024.f, 768.f))),
    player("../assets/ful_anim.png"),
    level(),        
    showDeathScreen(false),
    font(),            
    timerText(font, "", 24)
{
    // Загрузка текстур для уровня
    loadTexture(wallTexture, "../assets/block.png");
    level.setWallTexture(wallTexture);

    loadTexture(emptyTexture, "../assets/empty.png");
    level.setEmptyTexture(emptyTexture);

    loadTexture(backgroundTexture, "../assets/background.png");
    level.setBackgroundTexture(backgroundTexture);

    loadTexture(cloudsTexture, "../assets/clouds.png");
    level.setCloudTexture(cloudsTexture);

    loadTexture(spikesTexture, "../assets/spikes.png");
    level.setSpikeTexture(spikesTexture);

    // Загрузка текстуры окна смерти
    if (!deathWindowTexture.loadFromFile("../assets/death_window.png"))
    {
        throw std::runtime_error("failed to load death window texture");
    }
    deathWindowSprite = sf::Sprite(deathWindowTexture);
    // Рисуем окно смерти с левого верхнего угла
    deathWindowSprite.setPosition(sf::Vector2f(0.f, 0.f));

    // Загрузка данных уровня
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
    level.loadLevel(levelData);

    // Загрузка музыки
    if (!backgroundMusic.openFromFile("../assets/differentDay.mp3"))
    {
        throw std::runtime_error("failed to load music");
    }
    backgroundMusic.setVolume(0.0f);
    backgroundMusic.play();

    // Инициализация облаков: cloudsTexture уже загружен, задаём спрайт облаков
    cloudsSprite = sf::Sprite(*cloudsTexture);
    sf::Vector2u windowSize = window.getSize();
    float scaleX = static_cast<float>(windowSize.x) / cloudsTexture->getSize().x;
    float scaleY = static_cast<float>(windowSize.y) / cloudsTexture->getSize().y;
    cloudsSprite.setScale(sf::Vector2f(scaleX, scaleY));

    // Загрузка шрифта для текста
    if (!font.openFromFile("../assets/menu_font.ttf"))
    {
        throw std::runtime_error("failed to load font");
    }
    
    // Инициализация текста таймера
    timerText.setFont(font);
    timerText.setCharacterSize(26);
    timerText.setFillColor(sf::Color::White);
    timerText.setPosition(sf::Vector2f(10.f, 10.f));

    // Запускаем игровой таймер: gameTimer отсчитывает время с момента запуска игры
    gameTimer.restart();
}

void Game::loadTexture(std::shared_ptr<sf::Texture>& texture, const std::string& path) {
    texture = std::make_shared<sf::Texture>();
    if (!texture->loadFromFile(path)) {
        throw std::runtime_error("failed to load texture: " + path);
    }
}

void Game::run() {
    sf::Clock clock; // Для измерения deltaTime

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        handleEvents();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::K)) {
            player.kill();
        }

        if (player.get_isDead()) {
            showDeathScreen = true;
        }

        if (!showDeathScreen) {
            player.update(deltaTime, level);
        }

        updateCamera();
        updateTimer();  // Обновляем текст таймера
        render();
    }
}

void Game::handleEvents() {
    std::optional<sf::Event> eventOpt;
    while ((eventOpt = window.pollEvent())) {
        const sf::Event& event = *eventOpt;
        if (event.is<sf::Event::Closed>()) {
            window.close();
        }

        if (showDeathScreen) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
                player.respawn();
                showDeathScreen = false;
                gameTimer.restart();  // Перезапускаем таймер при возрождении
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
                window.close();
            }
        }
    }
}

void Game::updateCamera() {
    sf::Vector2f playerPos = player.getPosition();
    camera.setCenter(playerPos);
    window.setView(camera);
}

void Game::updateTimer() {
    float elapsed = gameTimer.getElapsedTime().asSeconds();
    int minutes = static_cast<int>(elapsed) / 60;
    int seconds = static_cast<int>(elapsed) % 60;
    int milliseconds = static_cast<int>((elapsed - static_cast<int>(elapsed)) * 1000);

    std::stringstream ss;
    ss << minutes << ":"
       << std::setw(2) << std::setfill('0') << seconds << ":"
       << std::setw(3) << std::setfill('0') << milliseconds;
    
    timerText.setString(ss.str());
}

void Game::render() {
    window.clear();

    // Рисуем фон (облака)
    window.setView(window.getDefaultView());
    window.draw(cloudsSprite);

    // Рисуем мир и игрока
    window.setView(camera);
    level.draw(window);
    player.draw(window);

    // Если игрок умер, рисуем окно смерти
    if (showDeathScreen) {
        window.setView(window.getDefaultView());
        window.draw(deathWindowSprite);
    }

     // Если экран победы активен — рисуем win screen
     else if (showWinScreen) {
        window.setView(window.getDefaultView());
        window.draw(winWindowSprite);
    }

    // Рисуем таймер (всегда в левом верхнем углу)
    window.setView(window.getDefaultView());
    window.draw(timerText);

    window.display();
}
