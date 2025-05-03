#include "../include/game.h"
#include "../include/player.h"
#include "../include/level.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <optional>
#include <memory>
#include <string>
#include <sstream>
#include <iomanip>

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
    timerText(font, "", 24), // Инициализация таймера
    dashText(font, "", 24)   // Инициализация таймера рывка
    
{
    // Создаем мобов с начальными позициями
    
    mobs.emplace_back("../assets/mob2.png", sf::Vector2f(600.f, 320.f), 220.f);  
    mobs.emplace_back("../assets/mob2.png", sf::Vector2f(600.f, 320.f), 520.f);    
    mobs.emplace_back("../assets/mob2.png", sf::Vector2f(600.f, 320.f), 320.f); 
    mobs.emplace_back("../assets/mob1.png", sf::Vector2f(600.f, 320.f), 420.f); 

    // Загрузка текстур для уровня
    loadTexture(wallTexture, "../assets/block.png");
    level.setWallTexture(wallTexture);

    loadTexture(emptyTexture, "../assets/empty.png");
    level.setEmptyTexture(emptyTexture);

    loadTexture(backgroundTexture, "../assets/background.png");
    level.setBackgroundTexture(backgroundTexture);

    loadTexture(cloudsTexture, "../assets/phone1.png");
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

    if (!winWindowTexture.loadFromFile("../assets/win_screen.png")) {
        throw std::runtime_error("failed to load win window texture");
    }
    winWindowSprite = sf::Sprite(winWindowTexture);
    winWindowSprite.setPosition(sf::Vector2f(0.f, 0.f));

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

    dashText.setFont(font);
    dashText.setCharacterSize(24);
    dashText.setFillColor(sf::Color::Yellow);
    dashText.setPosition(sf::Vector2f(10.f, 40.f)); // Размещаем под игровым таймером
    dashText.setString("Dash: 0:00");

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
            player.win();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::K)) {
            player.kill();
        }

        if (player.get_isWIn()) {
            showDeathScreen = true;
            gameTimer.stop();
        }

        if (player.get_isDead()) {
            showDeathScreen = true;
            gameTimer.stop();
        }

        if (!showDeathScreen && !showWinScreen) {
            update(deltaTime);

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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::V)) {
            showWinScreen = true;
        }

        if (showWinScreen) {
            gameTimer.stop();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
                player.respawn();
                showWinScreen = false;
                gameTimer.restart();  // Перезапускаем таймер при возрождении
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
                window.close();
            }
        }     

        if (showDeathScreen) {
            gameTimer.stop();
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
 
void Game::update(float deltaTime) {
    // Если игра находится в меню смерти или победы, пропускаем обновление
    if (showDeathScreen || showWinScreen) return;

    // Обновление игрока
    player.update(deltaTime, level);

    // Обновление мобов
    for (auto& mob : mobs) {
        mob.update(deltaTime, player, level);
    }

    // Проверка столкновения игрока с мобами
    for (const auto& mob : mobs) {
        /*
        sf::Vector2f playerPos = player.getBounds().position();
        sf::Vector2f playerSize = player.getBounds().size();
        sf::Vector2f mobPos = mob.getBounds().position();
        sf::Vector2f mobSize = mob.getBounds().size();

        if (playerPos.x < mobPos.x + mobSize.x &&
            playerPos.x + playerSize.x > mobPos.x &&
            playerPos.y < mobPos.y + mobSize.y &&
            playerPos.y + playerSize.y > mobPos.y) {

            player.kill();
            showDeathScreen = true;
            gameTimer.stop();
        }    
        */    
    }

    // Обновляем камеру
    updateCamera();

    // Обновляем таймеры
    updateTimer();
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

    for (auto& mob : mobs) {
        mob.draw(window);
    }  

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
    window.draw(dashText);  

    window.display();
    updateDashTimer();
}

void Game::updateDashTimer() {
    float dashRemaining = player.getDashCooldownRemaining();
    int seconds = static_cast<int>(dashRemaining);
    int milliseconds = static_cast<int>((dashRemaining - seconds) * 1000);

    std::stringstream ss;
    ss << "Dash: " << seconds << ":" 
       << std::setw(3) << std::setfill('0') << milliseconds;
    
    dashText.setString(ss.str());
}

