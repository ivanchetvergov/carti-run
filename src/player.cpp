#include "../include/player.h"
#include "../include/level.h"

#include <iostream>

Player::Player() {}

Player::Player(const std::string& texturePath) {
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "failed to load player texture from " << texturePath << "\n";
    }

    sprite = std::make_unique<sf::Sprite>(texture);

    // начальный кадр
    currentFrame = sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(frameWidth, frameHeight));
    sprite->setTextureRect(currentFrame);

    // масштабируем до 64x64
    float scaleFactorX = 64.f / frameWidth;
    float scaleFactorY = 64.f / frameHeight;
    sprite->setScale(sf::Vector2f(scaleFactorX, scaleFactorY));

    // origin в центре по горизонтали и внизу по вертикали
    sprite->setOrigin(sf::Vector2f(frameWidth / 2.f, 0));

    // стартовая позиция
    sprite->setPosition(spawnPoint);

    // модули
    physicsModule = std::make_unique<PhysicsModule>(gravity, jumpForce);
    collisionModule = std::make_unique<CollisionModule>();
}

void Player::update(float deltaTime, const Level& level) {

    if (isDead) return;

    InputManager inputManager;
    float horizontalInput = inputManager.getHorizontalInput();

    sf::Vector2f pos = sprite->getPosition();
    sf::Vector2f scale = sprite->getScale();
    sf::Vector2f spriteSize(frameWidth * std::abs(scale.x), frameHeight * std::abs(scale.y));

    // проверка на убийственную плитку
    int tileX = static_cast<int>(pos.x / level.getTileSize());
    int tileY = static_cast<int>(pos.y / level.getTileSize());
    if (level.isKilling(tileX, tileY)) {
        kill();
        return;
    }

    // прыжок
    if (inputManager.isJumpPressed() && isOnGround) {
        verticalSpeed = physicsModule->getJumpForce();
        isJumping = true;
        isOnGround = false;
    }

    float horizontalSpeed = horizontalInput * speed;

    // разворот
    float scaleFactorX = 64.f / frameWidth;
    float scaleFactorY = 64.f / frameHeight;

    if (horizontalInput > 0.f) {
        sprite->setScale(sf::Vector2f(std::abs(scaleFactorX), scaleFactorY));
        isFacingRight = true;
    } else if (horizontalInput < 0.f) {
        sprite->setScale(sf::Vector2f(-std::abs(scaleFactorX), scaleFactorY));
        isFacingRight = false;
    }

    // физика
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

    // сброс состояния
    isOnGround = false;

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

    // определение состояния
    if (!isOnGround) {
        state = (verticalSpeed > 0.f) ? PlayerState::Falling : PlayerState::Jumping;
    } else {
        if (horizontalInput != 0.f) {
            state = PlayerState::Walking;
        } else {
            state = PlayerState::Idle;
        }
    }

    sprite->setPosition(sf::Vector2f(candidateX, candidateY));
    updateAnimation(deltaTime);
}



void Player::updateAnimation(float deltaTime) {
    animationTimer += deltaTime;

    if (state == PlayerState::Walking) {
        if (animationTimer >= frameDuration) {
            animationTimer = 0.f;
            currentFrameIndex = (currentFrameIndex + 1) % walkFrameCount;
            currentFrame = sf::IntRect(
                sf::Vector2i(currentFrameIndex * frameWidth, 0),
                sf::Vector2i(frameWidth, frameHeight)
            );
            sprite->setTextureRect(currentFrame);
        }
    } else if (state == PlayerState::Jumping) {
        currentFrame = sf::IntRect(
            sf::Vector2i(jumpFrameX, jumpFrameY),
            sf::Vector2i(frameWidth, frameHeight)
        );
        sprite->setTextureRect(currentFrame);
    } else if (state == PlayerState::Idle) {
        currentFrame = sf::IntRect(
            sf::Vector2i(0, 0),
            sf::Vector2i(frameWidth, frameHeight)
        );
        sprite->setTextureRect(currentFrame);
    } else if (state == PlayerState::Falling) {
        currentFrame = sf::IntRect(
            sf::Vector2i(fallingFrameX, fallingFrameY),
            sf::Vector2i(frameWidth, frameHeight)
        );
        sprite->setTextureRect(currentFrame);
    }
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(*sprite);
}

void Player::respawn() {
    sprite->setPosition(spawnPoint);
    verticalSpeed = 0.f;
    isDead = false;
    isOnGround = false;
}
