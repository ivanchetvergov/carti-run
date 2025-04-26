#include "player.h"
#include "level.h"           // Уровень, с методами getTileSize() и isTileSolid(x, y)

#include <iostream>

Player::Player() {
    // Можно задать параметры по умолчанию
    physicsModule = std::make_unique<PhysicsModule>(980.f, -400.f); // gravity, jumpForce
    collisionModule = std::make_unique<CollisionModule>();
}

Player::Player(const std::string &texturePath) {
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "Failed to load player texture from " << texturePath << "\n";
    }
    sprite = std::make_unique<sf::Sprite>(texture);

    // Масштабирование до целевого размера (например, 64x64)
    float targetSize = 64.f;
    float scaleFactorX = targetSize / texture.getSize().x;
    float scaleFactorY = targetSize / texture.getSize().y;
    sprite->setScale(sf::Vector2f(scaleFactorX, scaleFactorY));
    sprite->setPosition({100.f, 100.f});
    
    physicsModule = std::make_unique<PhysicsModule>(2800.f, -1080.f);
    collisionModule = std::make_unique<CollisionModule>();
}

void Player::update(float deltaTime, const Level &level) {
    // Создаем объект InputManager на лету
    InputManager inputManager;
    float horizontalInput = inputManager.getHorizontalInput();
    
    // Если клавиша прыжка нажата и игрок на земле, запускаем прыжок
    if (inputManager.isJumpPressed() && isOnGround) {
        verticalSpeed = physicsModule->getJumpForce();
        isJumping = true;
        isOnGround = false;
    }
    
    // Рассчитываем горизонтальное движение на основе ввода
    float horizontalSpeed = horizontalInput * speed;
    
    // Интегрируем физику. Функция возвращает вектор скорости,
    // при этом verticalSpeed обновляется с учетом гравитации.
    sf::Vector2f velocity = physicsModule->integrate(deltaTime, horizontalSpeed, verticalSpeed);
    
    // Получаем текущую позицию и размеры спрайта.
    sf::Vector2f pos = sprite->getPosition();
    sf::Vector2u texSize = texture.getSize();
    sf::Vector2f scale = sprite->getScale();
    sf::Vector2f spriteSize(texSize.x * scale.x, texSize.y * scale.y);
    
    // Вычисляем кандидатное положение.
    float candidateX = pos.x + velocity.x * deltaTime;
    float candidateY = pos.y + velocity.y * deltaTime;
    
    // Проверка горизонтального движения
    if (horizontalSpeed < 0.f) { // Движемся влево
        if (collisionModule->checkLeftCollision(level, candidateX, pos, spriteSize)) {
            candidateX = pos.x;   // Отмена горизонтального смещения
            horizontalSpeed = 0.f;
        }
    } else if (horizontalSpeed > 0.f) { // Движемся вправо
        if (collisionModule->checkRightCollision(level, candidateX, pos, spriteSize)) {
            candidateX = pos.x;   // Отмена горизонтального смещения
            horizontalSpeed = 0.f;
        }
    }

    // Проверка вертикального движения
    if (verticalSpeed < 0.f) { // Движемся вверх (прыжок)
        if (collisionModule->checkTopCollision(level, candidateY, pos, spriteSize, verticalSpeed)) {
            // candidateY уже скорректировано внутри метода
        }
    } else if (verticalSpeed > 0.f) { // Движемся вниз (падение)
        if (collisionModule->checkBottomCollision(level, candidateY, pos, spriteSize, verticalSpeed, isOnGround)) {
            // candidateY уже скорректировано внутри метода
        }
    }
    
    // Обновляем позицию спрайта.
    sprite->setPosition(sf::Vector2f(candidateX, candidateY));

}

void Player::draw(sf::RenderWindow &window) {
    window.draw(*sprite);
}
