#include "player.h"
#include "level.h"
#include <iostream>

Player::Player() {}

Player::Player(const std::string& texturePath) {
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "failed to load player texture from " << texturePath << '\n';
    }
    sprite = std::make_unique<sf::Sprite>(texture);
    sprite->setPosition({100.f, 100.f});
}


void Player::handleInput(float deltaTime, const Level& level) {
    sf::Vector2f movement(0.f, 0.f);
    float speed = 100.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        movement.x -= speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        movement.x += speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        movement.y -= speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        movement.y += speed;

    sf::Vector2f newPos = sprite->getPosition() + movement * deltaTime;

    if (!level.isBlockedAt(newPos.x, newPos.y)) {
        sprite->setPosition(newPos);
    }
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(*sprite);
}
