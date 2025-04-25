#include "player.h"
#include "level.h"
#include <iostream>

Player::Player() {}

Player::Player(const std::string& texturePath) {
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "failed to load player texture from " << texturePath << '\n';
    }
    sprite = std::make_unique<sf::Sprite>(texture);

    float targetSize = 64.0f; // Целевой размер блока или игрока
    float scaleFactorX = targetSize / texture.getSize().x; // вычисляем коэффициент масштаба по ширине
    float scaleFactorY = targetSize / texture.getSize().y; // вычисляем коэффициент масштаба по высоте

    sprite->setScale(sf::Vector2f(scaleFactorX, scaleFactorY)); // масштабируем текстуру игрока
    sprite->setPosition({100.f, 100.f}); // начальная позиция игрока

}


void Player::handleInput(float deltaTime, const Level& level) {
    sf::Vector2f movement(0.f, 0.f);

    // Обработка клавиш
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        movement.y -= speed * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        movement.y += speed * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        movement.x -= speed * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        movement.x += speed * deltaTime;
    }

    // Текущая позиция спрайта
    sf::Vector2f pos = sprite->getPosition();
    
    // Вычисляем размеры спрайта вручную: 
    // Размер текстуры (в пикселях) умноженный на масштаб спрайта.
    sf::Vector2u texSize = texture.getSize(); // texture – член класса Player
    sf::Vector2f scale = sprite->getScale();
    sf::Vector2f spriteSize(static_cast<float>(texSize.x) * scale.x,
                            static_cast<float>(texSize.y) * scale.y);

    // Горизонтальная проверка движения
    sf::Vector2f newPosX(pos.x + movement.x, pos.y);
    bool canMoveX = true;
    if (movement.x > 0.f) {  // Движение вправо: проверяем правую сторону
        int tileX = static_cast<int>((newPosX.x + spriteSize.x) / level.getTileSize());
        int tileYTop = static_cast<int>(newPosX.y / level.getTileSize());
        int tileYBottom = static_cast<int>((newPosX.y + spriteSize.y - 1) / level.getTileSize());
        if ( level.isTileSolid(tileX, tileYTop) || level.isTileSolid(tileX, tileYBottom) ) {
            canMoveX = false;
        }
    } else if (movement.x < 0.f) {  // Движение влево: проверяем левую сторону
        int tileX = static_cast<int>(newPosX.x / level.getTileSize());
        int tileYTop = static_cast<int>(newPosX.y / level.getTileSize());
        int tileYBottom = static_cast<int>((newPosX.y + spriteSize.y - 1) / level.getTileSize());
        if ( level.isTileSolid(tileX, tileYTop) || level.isTileSolid(tileX, tileYBottom) ) {
            canMoveX = false;
        }
    }

    // Вертикальная проверка движения
    sf::Vector2f newPosY(pos.x, pos.y + movement.y);
    bool canMoveY = true;
    if (movement.y > 0.f) {  // Движение вниз: проверяем нижнюю грань
        int tileY = static_cast<int>((newPosY.y + spriteSize.y) / level.getTileSize());
        int tileXLeft = static_cast<int>(newPosY.x / level.getTileSize());
        int tileXRight = static_cast<int>((newPosY.x + spriteSize.x - 1) / level.getTileSize());
        if ( level.isTileSolid(tileXLeft, tileY) || level.isTileSolid(tileXRight, tileY) ) {
            canMoveY = false;
        }
    } else if (movement.y < 0.f) {  // Движение вверх: проверяем верхнюю грань
        int tileY = static_cast<int>(newPosY.y / level.getTileSize());
        int tileXLeft = static_cast<int>(newPosY.x / level.getTileSize());
        int tileXRight = static_cast<int>((newPosY.x + spriteSize.x - 1) / level.getTileSize());
        if ( level.isTileSolid(tileXLeft, tileY) || level.isTileSolid(tileXRight, tileY) ) {
            canMoveY = false;
        }
    }

    // Применяем перемещеине по осям: если перемещение по конкретной оси разрешено, двигаемся
    if (canMoveX)
        sprite->move(sf::Vector2f(movement.x, 0.f));
    if (canMoveY)
        sprite->move(sf::Vector2f(0.f, movement.y));
}


void Player::draw(sf::RenderWindow& window) {
    window.draw(*sprite);
}
