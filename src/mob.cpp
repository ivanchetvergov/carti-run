#include "../include/mob.h"

#include <SFML/Graphics/Rect.hpp>
#include <stdexcept>
#include <iostream>

Mob::Mob(std::shared_ptr<sf::Texture> texture, sf::Vector2f position, float speed)
    : texture(texture), speed(speed), movingRight(true)
{
    sprite = std::make_unique<sf::Sprite>(*texture);
    sprite->setOrigin(sf::Vector2f(64.f / 2.f, 0.f));
    sprite->setPosition(position);
}


void Mob::update(float deltaTime, Player& player, const Level& level) {
    float moveStep = speed * deltaTime;
    sf::Vector2f nextPos = sprite -> getPosition() + sf::Vector2f(movingRight ? moveStep : -moveStep, 0);

    // Проверка столкновения со стеной
    int tileX = static_cast<int>(nextPos.x / level.getTileSize());
    int tileY = static_cast<int>(nextPos.y / level.getTileSize());

    if (level.isWall(tileX, tileY)) {
        movingRight = !movingRight; // Меняем направление при столкновении
        sprite -> setScale(sf::Vector2f(movingRight ? 1.f : -1.f, 1.f)); // Флип направления
    } else {
        sprite -> move(sf::Vector2f(movingRight ? moveStep : -moveStep, 0));
    }

    sf::Vector2f mobPos = sprite->getPosition();
sf::Vector2f playerPos = player.getPosition();

float distance = std::sqrt(std::pow(playerPos.x - mobPos.x, 2) + std::pow(playerPos.y - mobPos.y, 2));

if (distance < 50.f) { // Радиус столкновения
    player.kill();
}    
    
}

void Mob::draw(sf::RenderWindow& window) {
    window.draw(*sprite);
}
