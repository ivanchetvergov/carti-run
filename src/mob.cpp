#include "mob.h"
#include <stdexcept>

#include <iostream>

Mob::Mob(const std::string& texturePath, sf::Vector2f position, float speed)
    : speed(speed), movingRight(true) {

        if (!texture.loadFromFile(texturePath)) {
            std::cerr << "failed to load player texture from " << texturePath << "\n";
        }
    
        sprite = std::make_unique<sf::Sprite>(texture);
        sprite->setOrigin(sf::Vector2f(64 / 2.f, 0));
    
        // стартовая позиция
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

    // Проверка столкновения с игроком
    if (false) {
        player.kill();  // Если моб касается игрока — убиваем его
    }
}

void Mob::draw(sf::RenderWindow& window) {
    window.draw(*sprite);
}
