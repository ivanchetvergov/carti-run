#include "player.h"
#include "level.h"

#include <iostream>

Player::Player() {}

Player::Player(const std::string &texturePath) {
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "failed to load player texture from " << texturePath << "\n";
    }
    sprite = std::make_unique<sf::Sprite>(texture);

    // задаём начальный кадр (позиция и размер)
    currentFrame = sf::IntRect({0, 0}, {frameWidth, frameHeight});
    sprite->setTextureRect(currentFrame);

    // масштабирование под 64x64
    float scaleFactorX = 64.f / frameWidth;
    float scaleFactorY = 64.f / frameHeight;
    sprite->setScale(sf::Vector2f(scaleFactorX, scaleFactorY));

    // origin по центру снизу
    sprite->setOrigin(sf::Vector2f(frameWidth / 2.f, 0));

    // начальная позиция
    sprite->setPosition(sf::Vector2f(100.f, 100.f));

    // модули
    physicsModule = std::make_unique<PhysicsModule>(gravity, jumpForce);
    collisionModule = std::make_unique<CollisionModule>();

    // параметры анимации
    frameDuration = 0.1f;
    currentFrameIndex = 0;
    walkFrameCount = 8;
}

void Player::update(float deltaTime, const Level &level) {
    InputManager inputManager;
    float horizontalInput = inputManager.getHorizontalInput();

    if (inputManager.isJumpPressed() && isOnGround) {
        verticalSpeed = physicsModule->getJumpForce();
        isJumping = true;
        isOnGround = false;
    }

    sf::Vector2f pos = sprite->getPosition();
    sf::Vector2f scale = sprite->getScale();
    sf::Vector2f spriteSize(frameWidth * std::abs(scale.x), frameHeight * std::abs(scale.y));

    float horizontalSpeed = horizontalInput * speed;

    float scaleFactorX = 64.f / frameWidth;
    float scaleFactorY = 64.f / frameHeight;

    // анимация при движении
    if (horizontalInput != 0.f) {
        animationTimer += deltaTime;
        if (animationTimer >= frameDuration) {
            animationTimer = 0.f;
            currentFrameIndex = (currentFrameIndex + 1) % walkFrameCount;
            currentFrame = sf::IntRect(
                sf::Vector2i(currentFrameIndex * frameWidth, 0),
                sf::Vector2i(frameWidth, frameHeight)
            );
            sprite->setTextureRect(currentFrame);
        }
    }

    // разворот спрайта
    if (horizontalInput > 0.f) {
        sprite->setScale(sf::Vector2f(std::abs(scaleFactorX), scaleFactorY));
    } else if (horizontalInput < 0.f) {
        sprite->setScale(sf::Vector2f(-std::abs(scaleFactorX), scaleFactorY));
    }

    sf::Vector2f velocity = physicsModule->integrate(deltaTime, horizontalSpeed, verticalSpeed);

    float candidateX = pos.x + velocity.x * deltaTime;
    float candidateY = pos.y + velocity.y * deltaTime;

    // коллизии по x
    if (horizontalSpeed < 0.f) {
        if (collisionModule->checkLeftCollision(level, candidateX, pos, spriteSize)) {
            candidateX = pos.x;
            horizontalSpeed = 0.f;
        }
    } else if (horizontalSpeed > 0.f) {
        if (collisionModule->checkRightCollision(level, candidateX, pos, spriteSize)) {
            candidateX = pos.x;
            horizontalSpeed = 0.f;
        }
    }

    // коллизии по y
    if (verticalSpeed < 0.f) {
        if (collisionModule->checkTopCollision(level, candidateY, pos, spriteSize, verticalSpeed)) {
            verticalSpeed = 0.f;
        }
    } else if (verticalSpeed > 0.f) {
        if (collisionModule->checkBottomCollision(level, candidateY, pos, spriteSize, verticalSpeed, isOnGround)) {
            verticalSpeed = 0.f;
        }
    }

    sprite->setPosition(sf::Vector2f(candidateX, candidateY));
}

void Player::draw(sf::RenderWindow &window) {
    window.draw(*sprite);
}
