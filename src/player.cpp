#include "../include/player.h"
#include "../include/level.h"

#include <iostream>

Player::Player() {}

Player::Player(const std::string& texturePath) {
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "failed to load player texture from " << texturePath << "\n";
    }

    if (!smokeTexture.loadFromFile("../assets/dash.png")) {
        throw std::runtime_error("Failed to load smoke texture");
    }

    sprite = std::make_unique<sf::Sprite>(texture);
    smoke = std::make_unique<sf::Sprite>(smokeTexture);

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
    // Если игрок мёртв или победил, прекращаем обновление
    if (isDead || isWin) return;

    // Получаем ввод через InputManager
    InputManager inputManager;
    float horizontalInput = inputManager.getHorizontalInput();
    bool shiftPressed = inputManager.isShiftPressed();
    bool jumpPressed = inputManager.isJumpPressed();

    // Получаем позицию и размеры игрока
    sf::Vector2f pos = sprite->getPosition();
    sf::Vector2f scale = sprite->getScale();
    sf::Vector2f spriteSize(frameWidth * std::abs(scale.x), frameHeight * std::abs(scale.y));

    // Проверка на убийственную плитку (игрок моментально умирает)
    int tileX = static_cast<int>(pos.x / level.getTileSize());
    int tileY = static_cast<int>(pos.y / level.getTileSize());
    if (level.isKilling(tileX, tileY)) {
        kill();
        return;
    }

    // Обрабатываем прыжок
    if (jumpPressed && isOnGround) {
        verticalSpeed = physicsModule->getJumpForce();
        isJumping = true;
        isOnGround = false;
    }

    // Обрабатываем рывок
    float currentSpeed = isDashing ? dashSpeed : speed;
    if (shiftPressed && cooldownClock.getElapsedTime().asSeconds() > dashCooldown) {
        isDashing = true;
        dashClock.restart();
        cooldownClock.restart();

        if (isFacingRight) {
            smoke->setScale(sf::Vector2f(1.f, 1.f)); // Обычный размер
            smoke->setPosition(sprite->getPosition() - sf::Vector2f(76.f, -10.f));
        } else {
            smoke->setScale(sf::Vector2f(-1.f, 1.f)); // Отражаем по горизонтали
            smoke->setPosition(sprite->getPosition() - sf::Vector2f(-76.f, -10.f));
        }
        showSmoke = true;
        smokeTimer.restart(); // Запускаем таймер
    }

    if (showSmoke && smokeTimer.getElapsedTime().asSeconds() > 0.15f) {
        showSmoke = false;
    }

    // Ограничиваем время рывка
    if (isDashing && dashClock.getElapsedTime().asSeconds() > dashTime) {
        isDashing = false;
    }

    float horizontalSpeed = horizontalInput * currentSpeed;

    // Разворачиваем спрайт в зависимости от направления
    float scaleFactorX = 64.f / frameWidth;
    float scaleFactorY = 64.f / frameHeight;
    
    if (horizontalInput > 0.f) {
        sprite->setScale(sf::Vector2f(std::abs(scaleFactorX), scaleFactorY));
        isFacingRight = true;
    } else if (horizontalInput < 0.f) {
        sprite->setScale(sf::Vector2f(-std::abs(scaleFactorX), scaleFactorY));
        isFacingRight = false;
    }

    // Обновляем физику движения
    sf::Vector2f velocity = physicsModule->integrate(deltaTime, horizontalSpeed, verticalSpeed);
    float candidateX = pos.x + velocity.x * deltaTime;
    float candidateY = pos.y + velocity.y * deltaTime;

    // Проверяем коллизии по X
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

    // Сбрасываем состояние "на земле"
    isOnGround = false;

    // Проверяем коллизии по Y
    if (verticalSpeed < 0.f) {
        if (collisionModule->checkTopCollision(level, candidateY, pos, spriteSize, verticalSpeed)) {
            verticalSpeed = 0.f;
        }
    } else if (verticalSpeed > 0.f) {
        if (collisionModule->checkBottomCollision(level, candidateY, pos, spriteSize, verticalSpeed, isOnGround)) {
            verticalSpeed = 0.f;
        }
    }

    // Определяем состояние игрока
    if (!isOnGround) {
        state = (verticalSpeed > 0.f) ? PlayerState::Falling : PlayerState::Jumping;
    } else {
        if (horizontalInput != 0.f) {
            state = PlayerState::Walking;
        } else {
            state = PlayerState::Idle;
        }
    }

    // Обновляем позицию игрока
    sprite->setPosition(sf::Vector2f(candidateX, candidateY));

    // Обновляем анимацию
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

    if (showSmoke) {
        window.draw(*smoke);
    }

    window.draw(*sprite);
}

void Player::respawn() {
    sprite->setPosition(spawnPoint);
    verticalSpeed = 0.f;
    isDead = false;
    isOnGround = false;
    isWin = false;
}

float Player::getDashCooldownRemaining() const {
    float remaining = dashCooldown - cooldownClock.getElapsedTime().asSeconds();
    return remaining > 0 ? remaining : 0;
}



