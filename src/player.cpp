#include "player.h"
#include "level.h"           // Уровень, с методами getTileSize() и isTileSolid(x, y)

#include <iostream>

Player::Player() {
}

Player::Player(const std::string &texturePath) {
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "Failed to load player texture from " << texturePath << "\n";
    }
    sprite = std::make_unique<sf::Sprite>(texture);

    // Устанавливаем первый кадр (используем позицию и размер)
    sf::Vector2<int> position(0, 0);
    sf::Vector2<int> size(23, 35);
    currentFrame = sf::IntRect(position, size); // Размер одного кадра
    sprite->setTextureRect(currentFrame);

   // Масштабируем каждый кадр до целевого размера (например, 64x64)
    float targetWidth = 64.f; // Желаемая ширина
    float targetHeight = 64.f; // Желаемая высота
    float scaleFactorX = targetWidth / 23; // Масштабируем до 64 пикселей по ширине
    float scaleFactorY = targetHeight / 35; // Масштабируеаум до 64 пикселей по высоте
    sprite->setScale(sf::Vector2f(scaleFactorX , scaleFactorY));
    sprite->setPosition({100.f, 100.f}); // Начальная позиция игрока
    sf::Vector2f origin(0, 0);
    sprite->setOrigin(origin);

    // Инициализация модулей физики и столкновений
    physicsModule = std::make_unique<PhysicsModule>(2800.f, -1080.f);
    collisionModule = std::make_unique<CollisionModule>();

    // Настройка анимации
    frameDuration = 0.1f;  // Длительность одного кадра (100 мс)
    currentFrameIndex = 0; // Начинаем с первого кадра
    numFrames = 8;         // Всего 8 кадров в раскадровке
}


void Player::update(float deltaTime, const Level &level) {
    InputManager inputManager;
    float horizontalInput = inputManager.getHorizontalInput();

    // Если клавиша прыжка нажата и игрок на земле, запускаем прыжок
    if (inputManager.isJumpPressed() && isOnGround) {
        verticalSpeed = physicsModule->getJumpForce();
        isJumping = true;
        isOnGround = false;
    }

    // Получаем текущую позицию и размеры спрайта
    sf::Vector2f pos = sprite->getPosition();
    sf::Vector2u texSize = texture.getSize();
    sf::Vector2f scale = sprite->getScale();
    sf::Vector2f spriteSize(texSize.x * std::abs(scale.x), texSize.y * std::abs(scale.y));

    // Рассчитываем горизонтальное движение на основе ввода
    float horizontalSpeed = horizontalInput * speed;

    float targetSize = 64.f;
    float scaleFactorX = targetSize / 23;
    float scaleFactorY = targetSize / 35;

    if (horizontalInput != 0.f){
        animationTimer += deltaTime;

        if (animationTimer >= frameDuration) {
            animationTimer = 0.f;
            currentFrameIndex = (currentFrameIndex + 1) % numFrames; // Следующий кадр

            // Смещение кадра по ширине
            sf::Vector2<int> position(currentFrameIndex * frameWidth, 0); // Положение кадра
            sf::Vector2<int> size(frameWidth, frameHeight);               // Размер кадра
            currentFrame = sf::IntRect(position, size);

            sprite->setTextureRect(currentFrame); // Применяем новую область текстуры
        }
    }


    // Добавляем разворот игрока на основе направления движения
    if (horizontalInput > 0.f) { // Движение вправо
        if (sprite->getScale().x < 0.f) { // Если игрок был повернут влево
            sprite->move(sf::Vector2f(spriteSize.x, 0.f)); // Компенсация смещения вправо
        }
        sprite->setScale(sf::Vector2f(std::abs(scaleFactorX), scaleFactorY));
    } else if (horizontalInput < 0.f) { // Движение влево
        if (sprite->getScale().x > 0.f) { // Если игрок был повернут вправо
            sprite->move(sf::Vector2f(-spriteSize.x, 0.f)); // Компенсация смещения влево
        }
        sf::Vector2f origin(16, 0);
        sprite->setOrigin(origin);
        sprite->setScale(sf::Vector2f(-std::abs(scaleFactorX), scaleFactorY));
    }


    sf::Vector2f velocity = physicsModule->integrate(deltaTime, horizontalSpeed,
                                                                verticalSpeed);

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
        }
    } else if (verticalSpeed > 0.f) { // Движемся вниз (падение)
        if (collisionModule->checkBottomCollision(level, candidateY, pos, spriteSize, verticalSpeed, isOnGround)) {
        }
    }

    // Обновляем позицию спрайта.
    sprite->setPosition(sf::Vector2f(candidateX, candidateY));
}

void Player::draw(sf::RenderWindow& window) {
    // Отрисовка спрайта
    window.draw(*sprite);
}
