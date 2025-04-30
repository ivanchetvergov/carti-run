/*
#include "game.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "player.h"
#include "level.h"

#include <iostream>
#include <vector>
#include <optional>
#include <memory>

Game::Game()
: window(sf::VideoMode({1024, 768}), "Carti run"),
  camera(sf::FloatRect(sf::Vector2f(0.f, 0.f), sf::Vector2f(1024.f, 768.f))),
  player("../assets/ful_anim.png"),
  showDeathScreen(false),
  cloudsSprite(),
  deathText()
{
    // загрузка текстур
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

    // загрузка уровня
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

    // музыка
    if (!backgroundMusic.openFromFile("../assets/differentDay.mp3")) {
        throw std::runtime_error("failed to load music");
    }
    backgroundMusic.setVolume(0.0f);
    backgroundMusic.play();

    // облака
    cloudsSprite.setTexture(*cloudsTexture);
    sf::Vector2u windowSize = window.getSize();
    float scaleX = static_cast<float>(windowSize.x) / cloudsTexture->getSize().x;
    float scaleY = static_cast<float>(windowSize.y) / cloudsTexture->getSize().y;
    cloudsSprite.setScale(sf::Vector2f(scaleX, scaleY));

    // текст
    if (!font.openFromFile("../assets/menu_font.ttf")) {
        throw std::runtime_error("failed to load font");
    }

    deathText.setFont(font);
    deathText.setString("YOU'RE DEAD!\npress [R] to respawn\npress [Q] to quit");
    deathText.setCharacterSize(40);
    deathText.setFillColor(sf::Color::Red);
    deathText.setPosition(sf::Vector2f(200.f, 200.f));
}

void Game::loadTexture(std::shared_ptr<sf::Texture>& texture, const std::string& path) {
    texture = std::make_shared<sf::Texture>();
    if (!texture->loadFromFile(path)) {
        throw std::runtime_error("failed to load texture: " + path);
    }
}

void Game::run() {
    sf::Clock clock;

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

void Game::render() {
    window.clear();

    // фон
    window.setView(window.getDefaultView());
    window.draw(cloudsSprite);

    // мир и игрок
    window.setView(camera);
    level.draw(window);
    player.draw(window);

    // если игрок умер
    if (showDeathScreen) {
        window.setView(window.getDefaultView());
        window.draw(deathText);
    }

    window.display();
}
*/